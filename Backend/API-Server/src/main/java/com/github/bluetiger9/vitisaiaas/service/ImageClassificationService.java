package com.github.bluetiger9.vitisaiaas.service;

import java.nio.file.Path;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

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

    public Object classifyImage(String model, Path tempImagePath) {
        try {
            final Future<String> result = vitisAIMicroApps.run(MicroApp.IMAGE_CLASSIFY,
                    Map.of(Parameter.IMAGE, tempImagePath.toString(), Parameter.MODEL, model));

            final Object resObj = objectMapper.readValue(result.get(), Object.class);

            log.info("MicroApp result: {}", resObj);
            return resObj;

        } catch (InterruptedException | ExecutionException | JsonProcessingException e) {
            throw new RuntimeException("action failed", e);
        }
    }

    public Object classifyImagesBatch(String model, List<Path> tempImagePath) {
        try {
            final List<String> pathList = tempImagePath.stream()
                    .map(Path::toString)
                    .collect(Collectors.toList());

            final Future<String> result = vitisAIMicroApps.run(MicroApp.IMAGE_CLASSIFY_BATCH,
                    Map.of(Parameter.IMAGES, pathList, Parameter.MODEL, model));

            final Object resObj = objectMapper.readValue(result.get(), Object.class);

            log.info("MicroApp result: {}", resObj);
            return resObj;

        } catch (InterruptedException | ExecutionException | JsonProcessingException e) {
            throw new RuntimeException("action failed", e);
        }
    }

    public Object faceDetect(String model, Path tempImagePath) {
        try {
            final Future<String> result = vitisAIMicroApps.run(MicroApp.FACE_DETECT,
                    Map.of(Parameter.IMAGE, tempImagePath.toString(), Parameter.MODEL, model));

            final Object resObj = objectMapper.readValue(result.get(), Object.class);

            log.info("MicroApp result: {}", resObj);
            return resObj;

        } catch (InterruptedException | ExecutionException | JsonProcessingException e) {
            throw new RuntimeException("action failed", e);
        }
    }

    public Object laneDetect(String model, Path tempImagePath) {
        try {
            final Future<String> result = vitisAIMicroApps.run(MicroApp.LANE_DETECT,
                    Map.of(Parameter.IMAGE, tempImagePath.toString(), Parameter.MODEL, model));

            final Object resObj = objectMapper.readValue(result.get(), Object.class);

            log.info("MicroApp result: {}", resObj);
            return resObj;

        } catch (InterruptedException | ExecutionException | JsonProcessingException e) {
            throw new RuntimeException("action failed", e);
        }
    }
}