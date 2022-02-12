package com.github.bluetiger9.vitisaiaas.util;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.springframework.scheduling.concurrent.CustomizableThreadFactory;

import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;

@AllArgsConstructor
@Slf4j
public class ProcessRunner {
	
    private final ExecutorService executor = Executors.newCachedThreadPool(new CustomizableThreadFactory("process-runner-"));
	
	public Future<String> exec(String... commandAndArguments) {
		try {
			log.info("Launching process: " + Arrays.toString(commandAndArguments));
			final ProcessBuilder processBuilder = new ProcessBuilder(commandAndArguments);			
			final Process process = processBuilder.start();			
			
			final CompletableFuture<String> result = new CompletableFuture<>();			
			executor.submit(new OutputCollector(process.getInputStream(), result));
			return result;
			
		} catch (Exception e) {
			throw new RuntimeException("Failed to execute process");
		}
	}
	
	@AllArgsConstructor
	public static class OutputCollector implements Runnable {
		private final InputStream input;
		private final CompletableFuture<String> result;
		
		@Override
		public void run() {
			final StringBuilder sb = new StringBuilder();
			try (var reader = new BufferedReader(new InputStreamReader(input))) {
				while (true) {
					final String line = reader.readLine();
                    if (line == null) {
                        log.info("Process exited!");
                        break;

                    } else if (line.matches("[A-Z][0-9][0-9][0-9][0-9] .*")) {
                        // log line
                        log.info(line);
                        continue;
                    }
					sb.append(line + "\n");
				}

				result.complete(sb.toString());

			} catch (IOException e) {
				log.error("Failed to read process output!", e);
				result.completeExceptionally(e);

			}
		}
	}

}
