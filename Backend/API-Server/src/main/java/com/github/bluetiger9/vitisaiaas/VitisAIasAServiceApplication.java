/*
 * Copyright © 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
package com.github.bluetiger9.vitisaiaas;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.mongodb.autoconfigure.MongoAutoConfiguration;
import org.springframework.boot.security.autoconfigure.actuate.web.servlet.ManagementWebSecurityAutoConfiguration;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configurers.AbstractHttpConfigurer;
import org.springframework.security.web.SecurityFilterChain;

@SpringBootApplication(exclude = {
        ManagementWebSecurityAutoConfiguration.class,
        MongoAutoConfiguration.class,
})
public class VitisAIasAServiceApplication {

    public static final long startTime = System.currentTimeMillis();

	public static void main(String[] args) {
		SpringApplication.run(VitisAIasAServiceApplication.class, args);
	}

	@Configuration
	@EnableWebSecurity
	public static class SecurityConfiguration {

	    @Bean
	    public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {
	        return http.csrf(AbstractHttpConfigurer::disable)
	            .authorizeHttpRequests(auth -> auth.anyRequest().permitAll())
	            .build();
	    }
	}

}
