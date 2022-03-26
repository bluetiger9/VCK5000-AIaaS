# Documentation for Artificial Intelligence as a Service (AIaaS) API

<a name="documentation-for-api-endpoints"></a>
## Documentation for API Endpoints

All URIs are relative to *http://x570-test.local:8080/api/v1*

Class | Method | HTTP request | Description
------------ | ------------- | ------------- | -------------
*ImagesApi* | [**imagesClassifyBatchPost**](Apis/ImagesApi.md#imagesclassifybatchpost) | **POST** /images/classify/batch | 
*ImagesApi* | [**imagesClassifyPost**](Apis/ImagesApi.md#imagesclassifypost) | **POST** /images/classify | 
*ImagesApi* | [**imagesFaceDetectPost**](Apis/ImagesApi.md#imagesfacedetectpost) | **POST** /images/face-detect | 
*ImagesApi* | [**imagesLaneDetectPost**](Apis/ImagesApi.md#imageslanedetectpost) | **POST** /images/lane-detect | 
*VideosApi* | [**videosYoloV3Post**](Apis/VideosApi.md#videosyolov3post) | **POST** /videos/yolo-v3 | 


<a name="documentation-for-models"></a>
## Documentation for Models

 - [FaceDetectItem](./Models/FaceDetectItem.md)
 - [FaceDetectResponse](./Models/FaceDetectResponse.md)
 - [FaceDetectResponse_data](./Models/FaceDetectResponse_data.md)
 - [ImageBatchClassifyItem](./Models/ImageBatchClassifyItem.md)
 - [ImageBatchClassifyResponse](./Models/ImageBatchClassifyResponse.md)
 - [ImageBatchClassifyResponse_data](./Models/ImageBatchClassifyResponse_data.md)
 - [ImageClassifyItem](./Models/ImageClassifyItem.md)
 - [ImageClassifyResponse](./Models/ImageClassifyResponse.md)
 - [ImageClassifyResponse_data](./Models/ImageClassifyResponse_data.md)
 - [LaneDetectItem](./Models/LaneDetectItem.md)
 - [LaneDetectItem_points](./Models/LaneDetectItem_points.md)
 - [LaneDetectResponse](./Models/LaneDetectResponse.md)
 - [LaneDetectResponse_data](./Models/LaneDetectResponse_data.md)
 - [YoloV3FrameItem](./Models/YoloV3FrameItem.md)
 - [YoloV3FrameItem_bboxes](./Models/YoloV3FrameItem_bboxes.md)
 - [YoloV3VideoResponse](./Models/YoloV3VideoResponse.md)
 - [YoloV3VideoResponse_data](./Models/YoloV3VideoResponse_data.md)


<a name="documentation-for-authorization"></a>
## Documentation for Authorization

All endpoints do not require authorization.
