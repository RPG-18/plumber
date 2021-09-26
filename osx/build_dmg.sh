#!/bin/zsh

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

macdeployqt build/plumber.app \
  -appstore-compliant \
  -executable=build/plumber.app/Contents/MacOS/plumber \
  -libpath=$HOME/Qt/6.1.2/macos/lib \
  -qmldir=$HOME/Qt/6.1.2/macos/qml \
  -fs=APFS -dmg