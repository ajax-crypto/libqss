FROM gitpod/workspace-full-vnc

RUN sudo apt-get update \
 && sudo apt-get install -y libgl1 \
 && sudo apt-get install -y libxkbcommon-x11-0 \
 && rm -rf /var/lib/apt/lists/*