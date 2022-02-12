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
import lombok.extern.slf4j.Slf4j;

@Service
@AllArgsConstructor
@Slf4j
public class ImageClassificationService {

    private final VitisAIMicroAppsService vitisAIMicroApps;
    private final ObjectMapper objectMapper;

    public Object classifyImage(Path tempImagePath, String model) {
        try {
            final Future<String> result = vitisAIMicroApps.run(MicroApp.IMAGE_CLASSIFY/* _DUMMY */,
                    Map.of(Parameter.IMAGE, tempImagePath.toString(), Parameter.MODEL, model));

            final Object resObj = objectMapper.readValue(result.get(), Object.class);

            log.info("MicroApp result: {}", resObj);
            return resObj;

        } catch (InterruptedException | ExecutionException | JsonProcessingException e) {
            throw new RuntimeException("action failed", e);
        }
    }
}