#!/usr/bin/env bash
set -e

IMAGE_NAME="rngdle-builder"
CONTAINER_NAME="rngdle_build"
OUT_DIR="$(pwd)/out"

echo "==> Step 1: Building Docker image: ${IMAGE_NAME}"
docker build -t "${IMAGE_NAME}" -f docker-build/Dockerfile .

echo "==> Step 2: Running build container"
docker run --name "${CONTAINER_NAME}" "${IMAGE_NAME}"

echo "==> Step 3: Copying artifact out of container"
mkdir -p "${OUT_DIR}"
# The Dockerfile copies the built artifact to /out inside the image; pull it from container
docker cp "${CONTAINER_NAME}:/out/rngdle_cli" "${OUT_DIR}/rngdle_cli"

echo "==> Step 4: Cleaning up container"
docker rm "${CONTAINER_NAME}" >/dev/null

echo "✔ Build successful!"
echo "✔ Output file: ${OUT_DIR}/rngdle_cli"
