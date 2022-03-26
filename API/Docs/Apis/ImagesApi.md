# ImagesApi

All URIs are relative to *http://x570-test.local:8080/api/v1*

Method | HTTP request | Description
------------- | ------------- | -------------
[**imagesClassifyBatchPost**](ImagesApi.md#imagesClassifyBatchPost) | **POST** /images/classify/batch | 
[**imagesClassifyPost**](ImagesApi.md#imagesClassifyPost) | **POST** /images/classify | 
[**imagesFaceDetectPost**](ImagesApi.md#imagesFaceDetectPost) | **POST** /images/face-detect | 
[**imagesLaneDetectPost**](ImagesApi.md#imagesLaneDetectPost) | **POST** /images/lane-detect | 


<a name="imagesClassifyBatchPost"></a>
# **imagesClassifyBatchPost**
> ImageBatchClassifyResponse imagesClassifyBatchPost(model, images)



### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **model** | **String**| Model name | [default to null]
 **images** | **List**|  | [optional] [default to null]

### Return type

[**ImageBatchClassifyResponse**](../Models/ImageBatchClassifyResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: multipart/form-data
- **Accept**: application/json

<a name="imagesClassifyPost"></a>
# **imagesClassifyPost**
> ImageClassifyResponse imagesClassifyPost(model, image)



### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **model** | **String**| Model name | [default to null]
 **image** | **File**|  | [optional] [default to null]

### Return type

[**ImageClassifyResponse**](../Models/ImageClassifyResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: multipart/form-data
- **Accept**: application/json

<a name="imagesFaceDetectPost"></a>
# **imagesFaceDetectPost**
> FaceDetectResponse imagesFaceDetectPost(model, image)



### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **model** | **String**| Model name | [default to null]
 **image** | **File**|  | [optional] [default to null]

### Return type

[**FaceDetectResponse**](../Models/FaceDetectResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: multipart/form-data
- **Accept**: application/json

<a name="imagesLaneDetectPost"></a>
# **imagesLaneDetectPost**
> LaneDetectResponse imagesLaneDetectPost(model, image)



### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **model** | **String**| Model name | [default to null]
 **image** | **File**|  | [optional] [default to null]

### Return type

[**LaneDetectResponse**](../Models/LaneDetectResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: multipart/form-data
- **Accept**: application/json

