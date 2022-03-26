# Artificial Intelligence as a Service (AIaaS) API

This `AIaaS-API.yml` contains the **OpenAPI 3.x Specification** of the **Artificial Intelligence as a Service (AIaaS) API**.

Tools like the [**OpenAPI Generator**](https://github.com/OpenAPITools/openapi-generator) can be used to generate **API Clients** and **Documentation** from this specification.

The `Docs/` folder contains the [**Documentation**](./Docs/README.md) of the API rendered in **Markdown** format.

## API-s

* **Images** - Image classification endpoints
* **Videos** - Video classification endpoints

## Documentation and API Client Generation

The [**OpenAPI Generator**](https://github.com/OpenAPITools/openapi-generator) can be used to generate **API Clients** and **Documentation** from the API specification file.

To generate **Documentation**:
```
java -jar openapi-generator-cli.jar generate -g markdown -i AIaaS-API.yml -o ./Docs
```

To generate **API Clients** for different **programming languages** and **frameworks**:
```
java -jar openapi-generator-cli.jar generate -g <generator> -i AIaaS-API.yml
```
*Please see the OpenAPI Generator documenatation for more details!*