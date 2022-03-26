package com.github.bluetiger9.vitisaiaas.service;

import java.io.IOException;
import java.nio.file.Path;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import com.github.bluetiger9.vitisaiaas.transfer.GenericResponse;
import com.github.bluetiger9.vitisaiaas.util.TempUtils;
import com.github.bluetiger9.vitisaiaas.util.TempUtils.TempDirResource;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.Setter;
import lombok.extern.slf4j.Slf4j;

@Service
@AllArgsConstructor
@Slf4j
public class APIService {

    private final ImageClassificationService imageClassificationService;
    private final VideoClassificationService videoClassificationService;

    private final List<Task> taskHistory = new CopyOnWriteArrayList<>();

    public List<Task> getTaskHistory() {
        return taskHistory;
    }

    public GenericResponse classifyImage(MultipartFile imageFile, String model) {
        final Task task = Task.builder()
                .id(taskId())
                .type("image-classify")
                .attributes(Map.of("model", model, "image", imageFile.getOriginalFilename()))
                .build();

        return runTask(task, tempDir -> {
            final Path tempImagePath = tempDir.getPath().resolve("image." + getFileExtension(imageFile, "jpg"));

            log.info("Image path: {}", tempImagePath.toAbsolutePath());
            imageFile.transferTo(tempImagePath);

            final Object result = imageClassificationService.classifyImage(model, tempImagePath);
            return new GenericResponse(result);
        });
    }

    public GenericResponse classifyImagesBatch(MultipartFile[] imageFiles, String model) {
        final Task task = Task.builder()
                .id(taskId())
                .type("image-batch-classify")
                .attributes(Map.of("model", model, "numImages", imageFiles.length))
                .build();

        return runTask(task, tempDir -> {
            final List<Path> tempImagePaths = new ArrayList<>();
            for (var imageFile : imageFiles) {
                final String normalizedFileName = getFilenameWithoutExtension(imageFile).replaceAll("[^a-zA-Z0-9_-]", "_");
                final Path tempImagePath = tempDir.getPath().resolve(normalizedFileName + "." + getFileExtension(imageFile, "jpg"));
                log.info("Image path: {}", tempImagePath.toAbsolutePath());

                imageFile.transferTo(tempImagePath);
                tempImagePaths.add(tempImagePath);
            }

            final Object result = imageClassificationService.classifyImagesBatch(model, tempImagePaths);
            return new GenericResponse(result);
        });
    }

    public GenericResponse faceDetect(MultipartFile imageFile, String model) {
        final Task task = Task.builder()
                .id(taskId())
                .type("face-detect")
                .attributes(Map.of("model", model, "image", imageFile.getOriginalFilename()))
                .build();

        return runTask(task, tempDir -> {
            final Path tempImagePath = tempDir.getPath().resolve("image." + getFileExtension(imageFile, "jpg"));

            log.info("Image path: {}", tempImagePath.toAbsolutePath());
            imageFile.transferTo(tempImagePath);

            final Object result = imageClassificationService.faceDetect(model, tempImagePath);
            return new GenericResponse(result);
        });
    }

    public GenericResponse laneDetect(MultipartFile imageFile, String model) {
        final Task task = Task.builder()
                .id(taskId())
                .type("lane-detect")
                .attributes(Map.of("model", model, "image", imageFile.getOriginalFilename()))
                .build();

        return runTask(task, tempDir -> {
            final Path tempImagePath = tempDir.getPath().resolve("image." + getFileExtension(imageFile, "jpg"));

            log.info("Image path: {}", tempImagePath.toAbsolutePath());
            imageFile.transferTo(tempImagePath);

            final Object result = imageClassificationService.laneDetect(model, tempImagePath);
            return new GenericResponse(result);
        });
    }

    public GenericResponse yoloV3VideoDetect(MultipartFile videoFile, String model) {
        final Task task = Task.builder()
                .id(taskId())
                .type("yolo-v3-video-detect")
                .attributes(Map.of("model", model, "video", videoFile.getOriginalFilename()))
                .build();

        return runTask(task, tempDir -> {
            final Path tempVideoPath = tempDir.getPath().resolve("video." + getFileExtension(videoFile, "mp4"));

            log.info("Video path: {}", tempVideoPath.toAbsolutePath());
            videoFile.transferTo(tempVideoPath);

            final Object result = videoClassificationService.yoloV3Detect(model, tempVideoPath);
            return new GenericResponse(result);
        });
    }

    public GenericResponse dummyAction(String text) {
        final Task task = Task.builder()
                .id(taskId())
                .type("dummy")
                .attributes(Collections.singletonMap("text", text))
                .build();
        return runTask(task, _ign -> new GenericResponse("OK"));
    }

    private GenericResponse runTask(Task task, TaskAction taskAction) {
        taskHistory.add(task);
        try (var tempDir = TempUtils.tempDirResource()) {            
            log.info("Temp dir: {}", tempDir.getPath());
            task.setState("IN-PROGRESS");
            task.details.put("tempDir", tempDir.getPath().toString());
            
            final GenericResponse result = taskAction.execute(tempDir);
            task.setState("FINISHED");
            return result;

        } catch (IOException e) {
            task.setState("FAILED");
            throw new RuntimeException("I/O error occurred", e);
            
        } finally {
            task.setFinishedDate(now());
            task.setDuration(task.getFinishedDate().toEpochSecond(ZoneOffset.UTC)
                    - task.getCreationDate().toEpochSecond(ZoneOffset.UTC));
        }
    }

    private static String getFileExtension(MultipartFile imageFile, String defaultValue) {
        try {
            final String[] tokens = imageFile.getOriginalFilename().split("\\.");
            return tokens[tokens.length - 1];

        } catch (Exception e) {
            return defaultValue;
        }
    }

    private static String getFilenameWithoutExtension(MultipartFile imageFile) {
        final String name = imageFile.getOriginalFilename();
        final String extension = getFileExtension(imageFile, "");
        return name.substring(0, name.length() - (extension.isBlank() ? 0 : extension.length() + 1));
    }

    private static String taskId() {
        return "t-" + System.currentTimeMillis();
    }
    
    private static LocalDateTime now() {
        return LocalDateTime.now(ZoneId.of("UTC"));
    }

    @Builder
    @Getter
    @Setter
    public static class Task {
        private final String id;
        private final String type;
        private final Map<String, Object> attributes;
        private final LocalDateTime creationDate = now();
        private final Map<String, Object> details = new HashMap<>();

        @Builder.Default
        private String state = "NEW";
        private LocalDateTime finishedDate;
        private long duration;
    }

    public static interface TaskAction {
        GenericResponse execute(TempDirResource tempDir) throws IOException;
    }
}