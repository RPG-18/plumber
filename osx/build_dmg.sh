#!/bin/zsh

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

mkdir -p build/plumber.app/Contents/Resources/qml/Qt/labs
cp -R $HOME/Qt/6.2.1/macos/qml/Qt/labs/platform build/plumber.app/Contents/Resources/qml/Qt/labs/platform

macdeployqt build/plumber.app \
  -appstore-compliant \
  -executable=build/plumber.app/Contents/MacOS/plumber \
  -libpath=$HOME/Qt/6.2.1/macos/lib \
  -qmldir=$HOME/Qt/6.2.1/macos/qml \
  -fs=APFS -dmg