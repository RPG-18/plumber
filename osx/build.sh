#!/bin/zsh

export PATH="/usr/local/bin:$PATH"
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
