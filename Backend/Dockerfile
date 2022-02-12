FROM xilinx/vitis-ai-cpu:latest

RUN apt-get update || true

RUN apt-get install -y openjdk-11-jdk

RUN apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /scratch/*

ADD --chown=vitis-ai-user:vitis-ai-group setup.sh /workspace/setup.sh