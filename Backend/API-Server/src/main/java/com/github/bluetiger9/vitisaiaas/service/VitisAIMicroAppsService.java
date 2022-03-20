package com.github.bluetiger9.vitisaiaas.service;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

import org.springframework.stereotype.Service;

import com.github.bluetiger9.vitisaiaas.util.ProcessRunner;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

@Service
@Slf4j
public class VitisAIMicroAppsService {
    private final ProcessRunner processRunner = new ProcessRunner();

    public Future<String> run(MicroApp microApp, Map<Parameter, Object> parameters) {
        log.info("Executing Vitis AI micro app: {}. Parameters: {}", microApp, parameters);
        return processRunner.exec(
                microApp.resolveArgs(parameters).toArray(new String[0]));
    }

    public static enum MicroApp {

        IMAGE_CLASSIFY(
                "/workspace/demo/Vitis-AI-Library/samples/VitisAI-MicroApps/vitis_ai_image_classify",
                "%model%", "%image%"),

        IMAGE_CLASSIFY_BATCH(
                "/workspace/demo/Vitis-AI-Library/samples/VitisAI-MicroApps/vitis_ai_image_classify_batch",
                "%model%", "%images%"),

        FACE_DETECT(
                "/workspace/demo/Vitis-AI-Library/samples/VitisAI-MicroApps/vitis_ai_face_detect",
                "%model%", "%image%"),

        LANE_DETECT(
                "/workspace/demo/Vitis-AI-Library/samples/VitisAI-MicroApps/vitis_ai_lane_detect",
                "%model%", "%image%"),
        ;

        private final List<String> args;

        private MicroApp(String... args) {
            this.args = Arrays.asList(args);
        }

        public List<String> resolveArgs(Map<Parameter, Object> parameters) {
            final List<String> resolvedArgs = new ArrayList<>();
            for (String arg : args) {
                final var resolvedArg = resolveArg(arg, parameters);
                resolvedArgs.addAll(resolvedArg);
            }
            return resolvedArgs;
        }

        @SuppressWarnings({ "unchecked", "rawtypes" })
        private static List<String> resolveArg(String arg, Map<Parameter, Object> parameters) {
            for (var entry : parameters.entrySet()) {
                final Parameter parameter = entry.getKey();
                final String pattern = parameter.getPattern();
                if (!arg.contains(pattern)) {
                    continue;
                }

                if (parameter.isMulti()) {
                    return ((List<String>) (List) entry.getValue()).stream()
                            .map(value -> arg.replaceAll(pattern, value))
                            .collect(Collectors.toList());

                } else {
                    return Collections.singletonList(arg.replaceAll(pattern, (String) entry.getValue()));
                }
            }

            return Collections.singletonList(arg);
        }
    }

    @AllArgsConstructor
    @Getter
    public static enum Parameter {
        MODEL("%model%", false),
        IMAGE("%image%", false),
        IMAGES("%images%", true);

        private final String pattern;
        private final boolean multi;
    }
}
