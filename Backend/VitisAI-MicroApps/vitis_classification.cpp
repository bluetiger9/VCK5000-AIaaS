/*
 * Copyright 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
#include <glog/logging.h>

#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vitis/ai/classification.hpp>

//#include "./process_result.hpp"

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

  for (auto &r : result.scores)
  {
    LOG_IF(INFO, true) << "r.index " << r.index << " "
                       << result.lookup(r.index) << " "
                       << "r.score " << r.score << " "
                       << std::endl;
  }
}

int main(int argc, char *argv[])
{
  std::string model = argv[1];
  std::string image_path = argv[2];
  classify(model, image_path);
  return 0;
}