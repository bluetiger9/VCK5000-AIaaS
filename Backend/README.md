# AIaaS on the VCK5000 - Backend Implementation

This folder contains the **Backend** implementation of the **Artificial Intelligence as a Service (AIaaS)** project.

The **Backend** implementation is split into two main components:
- **API Server** - a Spring Boot application implementing the REST API defined in the OpenAPI specification file
- **Vitis-AI MicroApps** - a set of simple C++ applications implementing different hardware accelerated **AI workloads**, using the **Vitis-AI framework**

For more details, please see the `/API-Server` and `/Vitis-AI-MicroApps` folders.

The `Backend` implementation is shipped as a **Docker image** based on the official **Vitis-AI Docker Images**. The `Dockerfile` from this folder can be used to build the Docker image.


