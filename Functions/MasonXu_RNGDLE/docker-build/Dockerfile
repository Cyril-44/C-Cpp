FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# ---- 1. Install system dependencies (cached unless Dockerfile changes)
RUN apt-get update && apt-get install -y \
    python3.8 python3.8-venv python3.8-dev libpython3.8 \
    python3-pip build-essential \
    && rm -rf /var/lib/apt/lists/*

# ---- 2. Create venv (cached)
RUN python3.8 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# ---- 3. Copy only requirements first (cached unless requirements.txt changes)
WORKDIR /src
COPY requirements.txt .

# ---- 4. Install Python dependencies (cached)
RUN pip install --upgrade pip && pip install -r requirements.txt

# ---- 5. Copy actual source code (only this layer changes frequently)
COPY . .

# ---- 6. Build with PyInstaller (only re-run when source changes)
# Use additional hooks directory so readchar metadata is bundled
RUN mkdir -p /hooks && cp /src/hook-readchar.py /hooks/ || true
RUN pyinstaller --onefile --name rngdle_cli --console --additional-hooks-dir=/hooks rngdle/main.py

# RUN ls -R .
# RUN ls -R dist || true

# ---- 7. Output artifact
RUN mkdir -p /out && cp dist/rngdle_cli /out/
