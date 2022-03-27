/*
 * Copyright © 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
package com.github.bluetiger9.vitisaiaas.service;

import java.nio.file.Path;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

import org.springframework.stereotype.Service;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.github.bluetiger9.vitisaiaas.service.VitisAIMicroAppsService.MicroApp;
import com.github.bluetiger9.vitisaiaas.service.VitisAIMicroAppsService.Parameter;

import lombok.AllArgsConstructor;

@Service
@AllArgsConstructor
public class VideoClassificationService {

    private final VitisAIMicroAppsService vitisAIMicroApps;
    private final ObjectMapper objectMapper;

    public Object yoloV3Detect(String model, Path tempVideoPath) {
        try {
            final Future<String> result = vitisAIMicroApps.run(MicroApp.VIDEO_YOLO_V3,
                    Map.of(Parameter.VIDEO, tempVideoPath.toString(), Parameter.MODEL, model));

            return objectMapper.readValue(result.get(), Object.class);

        } catch (InterruptedException | ExecutionException | JsonProcessingException e) {
            throw new RuntimeException("action failed", e);
        }
    }
}