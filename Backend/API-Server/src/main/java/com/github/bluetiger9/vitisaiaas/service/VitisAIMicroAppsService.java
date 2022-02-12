package com.github.bluetiger9.vitisaiaas.service;

import java.util.Arrays;
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

    public Future<String> run(MicroApp microApp, Map<Parameter, String> parameters) {
        log.info("Executing Vitis AI micro app: {}. Parameters: {}", microApp, parameters);
        return processRunner.exec(
                microApp.resolveArgs(parameters).toArray(new String[0]));
    }

    public static enum MicroApp {
        IMAGE_CLASSIFY_DUMMY("printf",
                "{\\n  \"results\": [\n"
                        + "    { \"idx\": 109, \"class\": \"brain coral\", \"score\": 0.982666 },\\n"
                        + "F0130 11:22:34.880532 some message\\n"
                        + "    { \"idx\": 973, \"class\": \"coral reef\", \"score\": 0.00850172 },\\n"
                        + "    { \"idx\": 955, \"class\": \"jackfruit\", \"score\": 0.00662115 }\\n"
                        + "  ]\\n}\\n"),
        
        IMAGE_CLASSIFY(
                "/workspace/demo/Vitis-AI-Library/samples/VitisAI-MicroApps/vitis_ai_image_classify",
                "%model%", "%image%");

        private final List<String> args;

        private MicroApp(String... args) {
            this.args = Arrays.asList(args);
        }

        public List<String> resolveArgs(Map<Parameter, String> parameters) {
            return args.stream()
                    .map(arg -> resolveArg(arg, parameters))
                    .collect(Collectors.toUnmodifiableList());
        }

        private static String resolveArg(String arg, Map<Parameter, String> parameters) {
            for (var entry : parameters.entrySet()) {
                arg = arg.replaceAll(entry.getKey().getPattern(), entry.getValue());
            }
            return arg;
        }
    }

    @AllArgsConstructor
    @Getter
    public static enum Parameter {
        IMAGE("%image%"),
        MODEL("%model%");

        private final String pattern;
    }
}
