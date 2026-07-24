Python port of original.cpp (module: rngdle)

Files created under rngdle/:
- badges.py : badge definitions (indices 1..153)
- utils.py : core badge-testing utilities (have_badge, quicktest, etc.)
- cli.py   : simple Linux-friendly CLI
- main.py  : entry point

Usage:
  python3 -m rngdle.main

Note: when building a frozen executable (PyInstaller) the package must import using absolute imports. If you see an ImportError like "attempted relative import with no known parent package" when running the built binary, ensure rngdle/main.py uses absolute imports (this repository's main.py is configured accordingly).


Build executable (local, Ubuntu 20.04+):
  1. Ensure Python 3 installed (Ubuntu 20.04 default python3 is fine).
  2. Run: ./build_exe.sh
  3. Result: ./rngdle_cli (standalone executable)

Reproducible Docker build (Ubuntu 20.04):
  docker build -t rngdle-builder -f docker-build/Dockerfile ..
  docker run --rm --name builder rngdle-builder
  # copy from container or use a bind mount /out to retrieve /out/rngdle_cli

Notes:
- build_exe.sh uses a venv and PyInstaller. Adjust requirements.txt if you added other deps.
- Test the produced executable in a real terminal (not an IDE pane) for correct TUI behavior.

This project is a modular Python port of original.cpp adapted for Linux terminals.
