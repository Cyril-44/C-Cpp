#!/usr/bin/bash
clang++-18 main.cpp -o obf -std=c++17 \
  `llvm-config-18 --cxxflags --ldflags --libs` \
  -lclang-cpp -g