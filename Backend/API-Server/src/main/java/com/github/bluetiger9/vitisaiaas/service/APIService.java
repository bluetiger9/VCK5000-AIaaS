package com.github.bluetiger9.vitisaiaas.service;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import com.github.bluetiger9.vitisaiaas.transfer.GenericResponse;
import com.github.bluetiger9.vitisaiaas.util.TempUtils;

import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;

@Service
@AllArgsConstructor
@Slf4j
public class APIService {

    private final ImageClassificationService imageClassificationService;

    public GenericResponse classifyImage(MultipartFile imageFile, String model) {
        try (var tempDir = TempUtils.tempDirResource()) {
            log.info("Temp dir: {}", tempDir.getPath());
            
            final Path tempImagePath = tempDir.getPath().resolve("image." + getFileExtension(imageFile, "jpg"));

            log.info("Image path: {}", tempImagePath.toAbsolutePath());
            imageFile.transferTo(tempImagePath);

            final Object result = imageClassificationService.classifyImage(model, tempImagePath);
            return new GenericResponse(result);

        } catch (IOException e) {
            throw new RuntimeException("I/O error occurred", e);
        }
    }

    public GenericResponse classifyImagesBatch(MultipartFile[] imageFiles, String model) {
        try (var tempDir = TempUtils.tempDirResource()) {
            log.info("Temp dir: {}", tempDir.getPath());

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

        } catch (IOException e) {
            throw new RuntimeException("I/O error occurred", e);
        }
    }

    public GenericResponse dummyAction(String text) {
        return new GenericResponse("OK");
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
}