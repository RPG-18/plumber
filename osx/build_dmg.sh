#!/bin/zsh

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

cp build/kafkaui osx/kafkaui.app/Contents/MacOS
macdeployqt osx/kafkaui.app \
  -appstore-compliant \
  -executable=$HOME/kafkaui/osx/kafkaui.app/Contents/MacOS/kafkaui \
  -libpath=$HOME/Qt/6.1.2/macos/lib \
  -qmldir=$HOME/Qt/6.1.2/macos/qml \
  -fs=APFS -dmg