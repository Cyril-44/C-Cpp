#!/usr/bin/env bash
set -euo pipefail
# Local build script: creates a virtualenv, installs deps, runs PyInstaller
# Usage: ./build_exe.sh

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
venv="$HERE/.venv_build"
rm -rf "$venv" build dist rngdle_cli.spec || true
python3 -m venv "$venv"
# shellcheck source=/dev/null
source "$venv/bin/activate"
python -m pip install --upgrade pip
pip install -r "$HERE/requirements.txt"
# Install PyInstaller last to avoid version conflicts
pip install pyinstaller

# Build one-file console executable named rngdle_cli
pyinstaller --onefile --name rngdle_cli --console rngdle/main.py

# Result: dist/rngdle_cli
echo "Built: dist/rngdle_cli"

# Optional: copy to project root
cp dist/rngdle_cli "$HERE/"
echo "Executable copied to: $HERE/rngdle_cli"

# Deactivate
deactivate

echo "Done. Test with: ./rngdle_cli" 
