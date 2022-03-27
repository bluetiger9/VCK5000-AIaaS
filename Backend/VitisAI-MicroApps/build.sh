#
# Copyright © 2022 Attila Tőkés.
#
# Licensed under the Apache License, Version 2.0 (the "License").
#
result=0 && pkg-config --list-all | grep opencv4 && result=1
if [ $result -eq 1 ]; then
	OPENCV_FLAGS=$(pkg-config --cflags --libs-only-L opencv4)
else
	OPENCV_FLAGS=$(pkg-config --cflags --libs-only-L opencv)
fi

DEBUG_FLAGS=${1:-}

CXX=${CXX:-g++}
for file in $(ls *.cpp); do
	filename=${file%.*}
	$CXX -std=c++17 -O2 -I. ${DEBUG_FLAGS} -o ${filename} ${file} \
		-lvitis_ai_library-classification -lvitis_ai_library-facedetect -lvitis_ai_library-lanedetect \
		-lvitis_ai_library-yolov3 \
		-lvitis_ai_library-dpu_task -lvitis_ai_library-xnnpp -lvitis_ai_library-model_config -lvitis_ai_library-math \
		-lvart-util -lxir -pthread -ljson-c -lglog ${OPENCV_FLAGS} \
		-lopencv_core -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui
done