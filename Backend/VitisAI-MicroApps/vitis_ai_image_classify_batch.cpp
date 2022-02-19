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
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vitis/ai/classification.hpp>

const std::string file_name(std::string full_path) {
  if (full_path.find("/") != std::string::npos) {
    return full_path.substr(full_path.rfind("/") + 1);
  }
  return full_path;
}

const char * lookup(vitis::ai::ClassificationResult &result, int idx) {
  auto name = result.lookup(idx);
  return name == NULL ? "null" : name;
}

void output_result(vitis::ai::ClassificationResult &result, std::string image_file) {
  std::cout << "  {" << std::endl << "    \"image\": \"" << file_name(image_file) << "\","
            << std::endl << "    \"results\": [" << std::endl;
  for (auto it = begin(result.scores); it != end(result.scores); ++it)
  {
    auto r = *it;
    std::cout << "        { \"idx\": " << r.index
         << ", \"class\": \"" << lookup(result, r.index) << "\""
         << ", \"score\": " << r.score << " }";
    if (it != end(result.scores) - 1) {
      std::cout << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "      ]" << std::endl << "   }";
}

void output_results(std::vector<vitis::ai::ClassificationResult> &results, std::vector<std::string> image_file_names) {
  std::cout << "{" << std::endl << "  \"results\": [" << std::endl;
  auto it2 = begin(image_file_names);
  for (auto it = begin(results); it != end(results); ++it, ++it2)
  {
    output_result(*it, *it2);
    if (it != end(results) - 1) {
      std::cout << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "  ]" << std::endl << "}" << std::endl;
}

void classify_batch(std::string model_name, std::vector<std::string> image_file_names)
{
  std::vector<cv::Mat> images;
  for (auto image_file_name : image_file_names) {
    cv::Mat image = cv::imread(image_file_name);
    if (image.empty())
    {
      LOG(FATAL) << "cannot load " << image_file_name << std::endl;
      abort();
    }
    images.push_back(image);
  }

  auto model = vitis::ai::Classification::create(model_name);
  auto results = model->run(images);
  output_results(results, image_file_names);
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);

  if (argc < 3) {
    LOG(FATAL) << "Usage: " << argv[0] << " <model> <image-path> ..." << std::endl;
    return -1;
  }

  LOG(INFO) << "Vitis AI Image Batch Classify MicroApp" << std::endl;;
  std::string model = argv[1];

  std::vector<string> image_paths;
  for (int i = 2; i < argc; ++i) {
    image_paths.push_back(argv[i]);
  }

#if !defined VITIS_DUMMY_RESULTS
  classify_batch(model, image_paths);
  return 0;

#else
  // dummy results mode
  vitis::ai::ClassificationResult result1;
  result1.width = 10;
  result1.height = 10;
  result1.scores.push_back({ 20, 0.999});
  result1.scores.push_back({ 42, 0.083});

  vitis::ai::ClassificationResult result2;
  result2.width = 10;
  result2.height = 10;
  result2.scores.push_back({ 11, 0.999});
  result2.scores.push_back({ 12, 0.483});

  std::vector<vitis::ai::ClassificationResult> results;
  results.push_back(result1);
  results.push_back(result2);
  output_results(results, image_paths);
  return 0 ;
#endif
}