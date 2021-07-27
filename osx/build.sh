#!/bin/zsh

export PATH="/usr/local/bin:$PATH"
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

cp build/kafkaui osx/kafkaui.app/Contents/MacOS
macdeployqt osx/kafkaui.app -appstore-compliant \
  -executable=osx/kafkaui.app/Contents/MacOS/kafkaui \
  -libpath=/usr/local/lib \
  -qmldir=/usr/local/share/qt/qml -fs=APFS -dmg \
  -verbose=3
