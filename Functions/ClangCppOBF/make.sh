#!/usr/bin/bash
clang++-18 main.cpp -o obf -std=c++17 \
  `llvm-config-18 --cxxflags --ldflags --libs` \
  -lclangTooling -lclangDriver -lclangSerialization \
  -lclangFrontend -lclangAST -lclangASTMatchers -lclangSema \
  -lclangEdit -lclangLex -lclangParse -lclangRewrite -lclangRewriteFrontend \
  -lclangBasic