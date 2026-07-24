# Dockerfile to build rngdle_cli on Ubuntu 20.04
FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    python3-venv \
    python3-pip \
    ca-certificates \
    build-essential \
    git \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
# Copy project into container
COPY .. /src

# Create venv and install deps
RUN python3 -m venv /opt/venv \
 && /opt/venv/bin/pip install --upgrade pip setuptools wheel \
 && /opt/venv/bin/pip install -r requirements.txt pyinstaller

# Build with PyInstaller
RUN /opt/venv/bin/pyinstaller --onefile --name rngdle_cli --console rngdle/main.py

# Copy artifact to /out for docker cp or bind mount
RUN mkdir -p /out && cp dist/rngdle_cli /out/

CMD ["/bin/bash"]
