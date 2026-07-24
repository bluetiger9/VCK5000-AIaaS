#
# Copyright © 2026 Attila Tőkés.
#
# Licensed under the Apache License, Version 2.0 (the "License").
#

FROM xilinx/vitis-ai-cpu:latest

RUN apt-get update || true

RUN wget -qO - https://packages.adoptium.net/artifactory/api/gpg/key/public | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/adoptium.gpg > /dev/null
RUN echo "deb https://packages.adoptium.net/artifactory/deb $(awk -F= '/^VERSION_CODENAME/{print$2}' /etc/os-release) main" | sudo tee /etc/apt/sources.list.d/adoptium.list
RUN sudo apt update -y && sudo apt-get install -y temurin-26-jdk

RUN apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /scratch/*

ADD --chown=vitis-ai-user:vitis-ai-group setup.sh /workspace/setup.sh