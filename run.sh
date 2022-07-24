#!/bin/bash
./compile.sh

if [ -f ./build/a.out ]; then
  ./build/a.out
fi
