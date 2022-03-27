# AIaaS on the VCK5000 - Vitis-AI Micro-Apps (C++)

This folder contains a set of **Vitis-AI "Micro-Apps"** implenting **hardware accelerated AI workloads**.

The applications are written in **C++** and uses the **Vitis-AI** framework.

The set of **hardware accelerated AI workloads** implemented so far are:
- **Image Classification** (`vitis_ai_image_classify.cpp`) - image classification with custom model (ex `resnet50`)
- **Image Batch Classification** (`vitis_ai_image_classify_batch.cpp`) - similar to the above but can classify multiple images at the same time - this is also a sample app for batch-processing
- **Image Face Detection** (`vitis_ai_image_face_detect.cpp`) - face detection on images
- **Image Lane Detection** (`vitis_ai_image_lane_detect.cpp`) - lane detection on images
- **Video YOLO V3 Object Detection** (`vitis_ai_video_yolov3.cpp`) - YOLO V3 object detection implemented on videos - this is also a sample app for video processing

To **build** the **Vitis-AI "Micro-Apps"** the `./build.sh` script should be called in the **Vitis-AI Docker container**.

The application can be **ran** on hardware with **Xilinx VCK5000** card installed.

For **testing** the apps can also built with the `./build.sh "-D VITIS_DUMMY_RESULTS"` command. This will result in the applications returning dummy results. This can be usefull to run the apps on non Xilinx hardware.