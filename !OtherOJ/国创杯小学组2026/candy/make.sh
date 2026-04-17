#!/usr/bin/env bash
set -e
echo Copying candy default data/...
cp -r data.default.d/* tests/
echo Copying candy default down/...
cp -r down.default.d/* down/

