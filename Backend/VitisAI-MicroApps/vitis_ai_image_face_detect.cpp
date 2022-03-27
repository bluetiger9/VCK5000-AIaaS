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
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vitis/ai/facedetect.hpp>

int denorm(int size, float value) {
  //std::cout << "size=" << size << "  value=" << value << "  res=" << std::round(size * value) << std::endl;
  return std::round(size * value); 
}

void output_result(vitis::ai::FaceDetectResult &result, int rows, int cols) {
  std::cout << "{" << std::endl << "  \"results\": [" << std::endl;  
  for (auto it = begin(result.rects); it != end(result.rects); ++it)
  {
    auto r = *it;
    std::cout << "    { \"x\": " << denorm(cols, r.x) << ", \"y\": " << denorm(rows, r.y)
         << ", \"width\": " << denorm(cols, r.width)  << ", \"height\": " << denorm(rows, r.height)
         << ", \"score\": " << r.score << " }";
    if (it != end(result.rects) - 1) {
      std::cout << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "  ]" << std::endl << "}" << std::endl;
}

void classify(std::string model_name, std::string image_file_name)
{
  auto model = vitis::ai::FaceDetect::create(model_name);
  auto image = cv::imread(image_file_name);
  if (image.empty())
  {
    LOG(FATAL) << "cannot load " << image_file_name << std::endl;
    abort();
  }

  auto result = model->run(image);
  output_result(result, image.rows, image.cols);
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);

  if (argc != 3) {
    LOG(FATAL) << "Usage: " << argv[0] << " <model> <image-path>" << std::endl;
    return -1;
  }

  LOG(INFO) << "Vitis AI Face Detect MicroApp" << std::endl;;
  std::string model = argv[1];
  std::string image_path = argv[2];

#if !defined VITIS_DUMMY_RESULTS
  classify(model, image_path);
  return 0;

#else
  // dummy results mode
  vitis::ai::FaceDetectResult result;
  result.rects.push_back({ 0.19375, 0.14375, 0.146875, 0.15625, 0.997199});
  result.rects.push_back({ 0.528125, 0.1, 0.14375, 0.1625, 0.994089});
  output_result(result, 450, 386);
  return 0 ;
#endif
}