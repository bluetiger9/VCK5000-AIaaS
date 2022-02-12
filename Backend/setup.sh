#!/bin/bash

# prepare models
sudo rm -rf /usr/share/vitis_ai_library/models
sudo ln -s /workspace/.tmp-vck5000-models /usr/share/vitis_ai_library/models

# run VCK 5000 setup script
source /workspace/setup/vck5000/setup.sh