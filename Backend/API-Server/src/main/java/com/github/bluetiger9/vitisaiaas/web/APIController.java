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
    public GenericResponse classifyImage(@RequestParam("image") MultipartFile imageFile, @RequestParam("model") String model) {
        log.info("Received image classify request: model={} image={}", model, imageFile.getOriginalFilename());
        return apiService.classifyImage(imageFile, model);
    }

    @GetMapping("/dummy")
    @ResponseBody
    public GenericResponse dummyAction(@RequestParam String text) {
        return apiService.dummyAction(text);
    }

}