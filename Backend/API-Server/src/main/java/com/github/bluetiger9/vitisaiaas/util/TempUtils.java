package com.github.bluetiger9.vitisaiaas.util;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

@Slf4j
public class TempUtils {
    private static final String TEMP_DIR_PREFIX = ".temp-vck500-aiaas-";

    public static TempDirResource tempDirResource() throws IOException {
        return new TempDirResource(TEMP_DIR_PREFIX);
    }

    @Getter
    public static class TempDirResource implements Closeable {

        private final Path path;

        public TempDirResource(String prefix) throws IOException {
            this.path = Files.createTempDirectory(prefix).toAbsolutePath();
        }

        @Override
        public void close() throws IOException {
            cleanUp(path.toFile());
        }

        private void cleanUp(File file) {
            log.info("Clean-up {}", file);
            if (file.isDirectory()) {
                for (File child : file.listFiles()) {
                    cleanUp(child);
                }
            }
            file.delete();
        }

    }

}
