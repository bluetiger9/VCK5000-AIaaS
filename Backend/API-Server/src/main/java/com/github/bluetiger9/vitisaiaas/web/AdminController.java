package com.github.bluetiger9.vitisaiaas.web;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.List;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import com.github.bluetiger9.vitisaiaas.VitisAIasAServiceApplication;
import com.github.bluetiger9.vitisaiaas.service.APIService;
import com.github.bluetiger9.vitisaiaas.service.APIService.Task;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

@Controller
@RequestMapping("/admin")
@AllArgsConstructor
@Slf4j
public class AdminController {

    private final APIService apiService;

    @GetMapping("/server-info")
    @ResponseBody
    public ServerInfo serverInfo() throws UnknownHostException {
        log.info("Received server info request.");

        return new ServerInfo("VitisAI AIasS Server", "v0.0.1",
                ((double) System.currentTimeMillis() - VitisAIasAServiceApplication.startTime) / 1000.0,
                InetAddress.getLocalHost().getHostName(),
                System.getProperty("user.name"));
    }

    @GetMapping("/tasks")
    @ResponseBody
    public List<Task> taskList() throws UnknownHostException {
        log.info("Received task list request.");

        return apiService.getTaskHistory();
    }

    @GetMapping("/tasks/{id}")
    @ResponseBody
    public Task taskList(@PathVariable("id") String id) throws UnknownHostException {
        log.info("Received task request, id={}", id);

        return apiService.getTaskHistory().stream()
                .filter(task -> task.getId().equals(id))
                .findAny()
                .orElseThrow();
    }

    @AllArgsConstructor
    @Getter
    public class ServerInfo {
        private final String name;
        private final String version;
        private final double upTime;
        private final String hostname;
        private final String username;
    }

}