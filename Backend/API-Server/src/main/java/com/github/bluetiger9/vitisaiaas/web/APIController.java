package com.github.bluetiger9.vitisaiaas.web;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import com.github.bluetiger9.vitisaiaas.service.APIService;
import com.github.bluetiger9.vitisaiaas.transfer.GenericResponse;

import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;

@Controller
@RequestMapping("/api/v1")
@AllArgsConstructor
@Slf4j
public class APIController {

    private final APIService apiService;

    @PostMapping("/images/classify")
    @ResponseBody
    public GenericResponse classifyImage(@RequestParam("model") String model, @RequestParam("image") MultipartFile imageFile) {
        log.info("Received image classify request: model={} image={}", model, imageFile.getOriginalFilename());
        return apiService.classifyImage(imageFile, model);
    }

    @PostMapping("/images/classify/batch")
    @ResponseBody
    public GenericResponse classifyImage(@RequestParam("model") String model, @RequestParam("images") MultipartFile[] imageFiles) {
        log.info("Received image classify request: model={} image={}", model, imageFiles);
        return apiService.classifyImagesBatch(imageFiles, model);
    }

    @PostMapping("/images/face-detect")
    @ResponseBody
    public GenericResponse faceDetect(@RequestParam("model") String model, @RequestParam("image") MultipartFile imageFile) {
        log.info("Received face detect request: model={} image={}", model, imageFile.getOriginalFilename());
        return apiService.faceDetect(imageFile, model);
    }

    @PostMapping("/images/lane-detect")
    @ResponseBody
    public GenericResponse laneDetect(@RequestParam("model") String model, @RequestParam("image") MultipartFile imageFile) {
        log.info("Received lane detect request: model={} image={}", model, imageFile.getOriginalFilename());
        return apiService.laneDetect(imageFile, model);
    }

    @PostMapping("/videos/yolo-v3")
    @ResponseBody
    public GenericResponse yoloV3DetectVideo(@RequestParam("model") String model, @RequestParam("video") MultipartFile videoFile) {
        log.info("Received YOLO V3 Video request: model={} image={}", model, videoFile.getOriginalFilename());
        return apiService.yoloV3VideoDetect(videoFile, model);
    }

    @GetMapping("/dummy")
    @ResponseBody
    public GenericResponse dummyAction(@RequestParam String text) {
        return apiService.dummyAction(text);
    }

}