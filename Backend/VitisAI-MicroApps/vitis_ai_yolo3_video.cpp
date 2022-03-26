/*
 * Copyright 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
#include <cstddef>
#include <glog/logging.h>

#include <iostream>
#include <ostream>
#include <memory>
#include <cmath>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <vitis/ai/lanedetect.hpp>
#include <vitis/ai/bounded_queue.hpp>
#include <vitis/ai/env_config.hpp>
#include <vitis/ai/yolov3.hpp>

DEF_ENV_PARAM(DEBUG_DEMO, "0")
DEF_ENV_PARAM(DEMO_USE_VIDEO_WRITER, "0")
DEF_ENV_PARAM_2(
    DEMO_VIDEO_WRITER,
    "appsrc ! videoconvert ! queue ! kmssink "
    "driver-name=xlnx plane-id=39 fullscreen-overlay=false sync=false",
    std::string)
DEF_ENV_PARAM(DEMO_VIDEO_WRITER_WIDTH, "640")
DEF_ENV_PARAM(DEMO_VIDEO_WRITER_HEIGHT, "480")
DEF_ENV_PARAM(SAMPLES_ENABLE_BATCH, "1");
DEF_ENV_PARAM(SAMPLES_BATCH_NUM, "0");


// A struct that can storage data and info for each frame
struct FrameInfo {
  int channel_id;
  unsigned long frame_id;
  cv::Mat mat;
  float max_fps;
  float fps;
  int belonging;
  int mosaik_width;
  int mosaik_height;
  int horizontal_num;
  int vertical_num;
  cv::Rect_<int> local_rect;
  cv::Rect_<int> page_layout;
  std::string channel_name;
};

struct Filter {
  explicit Filter() {}
  virtual ~Filter() {}
  virtual cv::Mat run(int frameNr, cv::Mat& input) = 0;
};

// Execute each lib run function and processor your implement
template <typename dpu_model_type_t, typename ProcessResult>
struct DpuFilter : public Filter {
  DpuFilter(std::unique_ptr<dpu_model_type_t>&& dpu_model,
            const ProcessResult& processor)
      : Filter{}, dpu_model_{std::move(dpu_model)}, processor_{processor} {
    LOG(INFO) << "DPU model size=" << dpu_model_->getInputWidth() << "x"
              << dpu_model_->getInputHeight();
  }
  virtual ~DpuFilter() {}
  cv::Mat run(int frameNr, cv::Mat& image) override {
    auto result = dpu_model_->run(image);
    processor_(frameNr, image.cols, image.rows, result);
    return image;
  }
  std::unique_ptr<dpu_model_type_t> dpu_model_;
  const ProcessResult& processor_;
};
template <typename FactoryMethod, typename ProcessResult>
std::unique_ptr<Filter> create_dpu_filter(const FactoryMethod& factory_method,
                                          const ProcessResult& process_result) {
  using dpu_model_type_t = typename decltype(factory_method())::element_type;
  return std::unique_ptr<Filter>(new DpuFilter<dpu_model_type_t, ProcessResult>(
      factory_method(), process_result));
}

/* Threads */

using queue_t = vitis::ai::BoundedQueue<FrameInfo>;

/* BASE THREAD */
struct BaseThread {
  // static std::vector<BaseThread *> all_threads_;
  static inline std::vector<BaseThread*>& all_threads() {
    static std::vector<BaseThread*> threads;
    return threads;
  };
  static void signal_handler(int) { stop_all(); }
  static void stop_all() {
    for (auto& th : all_threads()) {
      th->stop();
    }
  }
  static void wait_all() {
    for (auto& th : all_threads()) {
      th->wait();
    }
  }
  static void start_all() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "Thread num " << all_threads().size();
    for (auto& th : all_threads()) {
      th->start();
    }
  }

  static void main_proxy(BaseThread* me) { return me->main(); }
  void main() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] is started";
    while (!stop_) {
      auto run_ret = run();
      if (!stop_) {
        stop_ = run_ret != 0;
      }
    }
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "thread [" << name() << "] is ended";
  }

  virtual int run() = 0;

  virtual std::string name() = 0;

  explicit BaseThread() : stop_(false), thread_{nullptr} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT A Thread";
    all_threads().push_back(this);
  }

  virtual ~BaseThread() {  //
    all_threads().erase(
        std::remove(all_threads().begin(), all_threads().end(), this),
        all_threads().end());
  }

  void start() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] is starting";
    thread_ = std::unique_ptr<std::thread>(new std::thread(main_proxy, this));
  }

  void stop() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] is stopped.";
    stop_ = true;
  }

  void wait() {
    if (thread_ && thread_->joinable()) {
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
          << "waiting for [" << name() << "] ended";
      thread_->join();
    }
  }
  bool is_stopped() { return stop_; }

  bool stop_;
  std::unique_ptr<std::thread> thread_;
};

/* DECODE THREAD */
struct DecodeThread : public BaseThread {
  DecodeThread(int channel_id, const std::string& video_file, queue_t* queue)
      : BaseThread{},
        channel_id_{channel_id},
        video_file_{video_file},
        frame_id_{0},
        video_stream_{},
        queue_{queue} {
    open_stream();
    auto& cap = *video_stream_.get();
    if (is_camera_) {
      cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
      cap.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
    }
  }

  virtual ~DecodeThread() {}

  virtual int run() override {
    auto& cap = *video_stream_.get();
    cv::Mat image;
    cap >> image;
    auto video_ended = image.empty();
    if (video_ended) {
      // loop the video
      stop_all();
      return -1;
      //open_stream();
      //return 0;
    }
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "decode queue size " << queue_->size();
    if (queue_->size() > 0 && is_camera_ == true) {
      return 0;
    }
    while (!queue_->push(FrameInfo{channel_id_, ++frame_id_, image},
                         std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
    return 0;
  }

  virtual std::string name() override {
    return std::string{"DedodeThread-"} + std::to_string(channel_id_);
  }

  void open_stream() {
    is_camera_ = video_file_.size() == 1 && video_file_[0] >= '0' &&
                 video_file_[0] <= '9';
    video_stream_ = std::unique_ptr<cv::VideoCapture>(
        is_camera_ ? new cv::VideoCapture(std::stoi(video_file_))
                   : new cv::VideoCapture(video_file_));
    if (!video_stream_->isOpened()) {
      LOG(ERROR) << "cannot open file " << video_file_;
      stop();
    }
  }

  int channel_id_;
  std::string video_file_;
  unsigned long frame_id_;
  std::unique_ptr<cv::VideoCapture> video_stream_;
  queue_t* queue_;
  bool is_camera_;
};

/** DPU THREAD */

struct DpuThread : public BaseThread {
  DpuThread(std::unique_ptr<Filter>&& filter, queue_t* queue_in, const std::string& suffix)
      : BaseThread{},
        filter_{std::move(filter)},
        queue_in_{queue_in},
        suffix_{suffix} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT DPU";
  }
  virtual ~DpuThread() {}

  virtual int run() override {
    FrameInfo frame;
    if (!queue_in_->pop(frame, std::chrono::milliseconds(500))) {
      return 0;
    }
    if (filter_) {
      frame.mat = filter_->run(frame.frame_id, frame.mat);
    }
    if (is_stopped()) {
      return -1;
    }
    return 0;
  }

  virtual std::string name() override { return std::string("DPU-") + suffix_; }
  std::unique_ptr<Filter> filter_;
  queue_t* queue_in_;
  std::string suffix_;
};

static bool firstFrame = true;
void output_result(int frame, int cols, int rows, const vitis::ai::YOLOv3Result &result) {
  if (firstFrame) {
    std::cout << std::endl;
    firstFrame = false;

  } else {
    std::cout << ", " << std::endl;
  }

  bool first = true;
  std::cout << "  { \"frame\": " << frame << ", \"bboxes\": [";
  for (auto it = begin(result.bboxes); it != end(result.bboxes); ++it) {
    auto bbox = *it;
    int label = bbox.label;
    float xmin = bbox.x * cols + 1;
    float ymin = bbox.y * rows + 1;
    float xmax = xmin + bbox.width * cols;
    float ymax = ymin + bbox.height * rows;
    float confidence = bbox.score;
    if (xmax > cols) xmax = cols;
    if (ymax > rows) ymax = rows;

    if (first) {
      std::cout << std::endl;

    } else {
      std::cout << "," << std::endl;
    }

    std::cout << "    { \"label\": " << bbox.label
        << ", \"x\": " << xmin << ", \"y\": " << ymin
        << ", \"width\": " << (xmax - xmin) << ", \"height\": " << (ymax - ymin)
        << ", \"score\": " << bbox.score  << " }"/* << std::endl*/;

    first = false;
  }

  std::cout << std::endl << "] }";
}

void classify(std::string model_name, std::string video_file_name)
{
  signal(SIGINT, BaseThread::signal_handler);

  cv::VideoCapture video_cap(video_file_name);

  auto channel_id = 0;
  auto decode_queue = std::unique_ptr<queue_t>{new queue_t{5}};
  auto decode_thread = std::unique_ptr<DecodeThread>(
      new DecodeThread{channel_id, video_file_name, decode_queue.get()});
  auto dpu_thread = std::vector<std::unique_ptr<DpuThread>>{};
  for (int i = 0; i < 1 /*4*/ /*g_num_of_threads[0]*/; ++i) {
    dpu_thread.emplace_back(new DpuThread(
        create_dpu_filter([model_name] {
          return vitis::ai::YOLOv3::create(model_name);
        }, output_result), decode_queue.get(), std::to_string(i)) );
  }

  std::cout << "{ \"frames\": [";

  BaseThread::start_all();
  BaseThread::wait_all();

  std::cout << std::endl << "] }";
  //std::cout << "Processed frames: " << processed_frames << std::endl;
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);

  if (argc != 3) {
    LOG(FATAL) << "Usage: " << argv[0] << " <model> <image-path>" << std::endl;
    return -1;
  }

  LOG(INFO) << "Vitis AI Lane Detect MicroApp" << std::endl;;
  std::string model = argv[1];
  std::string image_path = argv[2];

#if !defined VITIS_DUMMY_RESULTS
  classify(model, image_path);
  return 0;

#else
  // dummy results mode
  Dummy results not implemented for video...
  
#endif
}