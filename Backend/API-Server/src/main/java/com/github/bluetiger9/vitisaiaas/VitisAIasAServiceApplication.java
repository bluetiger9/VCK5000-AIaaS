package com.github.bluetiger9.vitisaiaas;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.actuate.autoconfigure.security.servlet.ManagementWebSecurityAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;

@SpringBootApplication(exclude = { SecurityAutoConfiguration.class, ManagementWebSecurityAutoConfiguration.class })
public class VitisAIasAServiceApplication {

    public static final long startTime = System.currentTimeMillis();

	public static void main(String[] args) {
		SpringApplication.run(VitisAIasAServiceApplication.class, args);
	}

//	@Configuration
//	@EnableWebSecurity
//	public class SecurityConfiguration extends WebSecurityConfigurerAdapter {
//		@Override
//		protected void configure(HttpSecurity http) throws Exception {
//			http.authorizeRequests().antMatchers("/").permitAll();
//		}
//	}

}
