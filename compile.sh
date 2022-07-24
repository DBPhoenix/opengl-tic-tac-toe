#!/bin/bash
if [ ! -d ./build ]; then
  mkdir ./build
fi

g++ -o ./build/glad.o -c ./src/glad.c -I ./include
ar rcs ./build/lib.a ./build/*

g++ -o ./build/a.out main.cpp ./src/glad.c -I./include -l glfw
