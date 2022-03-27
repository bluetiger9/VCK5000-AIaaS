/*
 * Copyright © 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
#include <cstddef>
#include <glog/logging.h>

#include <iostream>
#include <ostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vitis/ai/classification.hpp>

const char * lookup(vitis::ai::ClassificationResult &result, int idx) {
  auto name = result.lookup(idx);
  return name == NULL ? "null" : name;
}

void output_result(vitis::ai::ClassificationResult &result) {
  std::cout << "{" << std::endl << "  \"results\": [" << std::endl;
  for (auto it = begin(result.scores); it != end(result.scores); ++it)
  {
    auto r = *it;
    std::cout << "    { \"idx\": " << r.index
         << ", \"class\": \"" << lookup(result, r.index) << "\""
         << ", \"score\": " << r.score << " }";
    if (it != end(result.scores) - 1) {
      std::cout << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "  ]" << std::endl << "}" << std::endl;
}

void classify(std::string model_name, std::string image_file_name)
{
  auto model = vitis::ai::Classification::create(model_name);
  auto image = cv::imread(image_file_name);
  if (image.empty())
  {
    LOG(FATAL) << "cannot load " << image_file_name << std::endl;
    abort();
  }

  auto result = model->run(image);
  output_result(result);
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);

  if (argc != 3) {
    LOG(FATAL) << "Usage: " << argv[0] << " <model> <image-path>" << std::endl;
    return -1;
  }

  LOG(INFO) << "Vitis AI Image Classify MicroApp" << std::endl;;
  std::string model = argv[1];
  std::string image_path = argv[2];

#if !defined VITIS_DUMMY_RESULTS
  classify(model, image_path);
  return 0;

#else
  // dummy results mode
  vitis::ai::ClassificationResult result;
  result.width = 10;
  result.height = 10;
  result.scores.push_back({ 20, 0.999});
  result.scores.push_back({ 42, 0.083});
  output_result(result);
  return 0 ;
#endif
}