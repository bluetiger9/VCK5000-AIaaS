# VideosApi

All URIs are relative to *http://x570-test.local:8080/api/v1*

Method | HTTP request | Description
------------- | ------------- | -------------
[**videosYoloV3Post**](VideosApi.md#videosYoloV3Post) | **POST** /videos/yolo-v3 | 


<a name="videosYoloV3Post"></a>
# **videosYoloV3Post**
> YoloV3VideoResponse videosYoloV3Post(model, video)



### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **model** | **String**| Model name | [default to null]
 **video** | **File**|  | [optional] [default to null]

### Return type

[**YoloV3VideoResponse**](../Models/YoloV3VideoResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: multipart/form-data
- **Accept**: application/json

