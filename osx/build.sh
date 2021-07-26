#!/bin/zsh

export PATH="/usr/local/bin:$PATH"
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

cp build/kafkaui osx/kafkaui.app/Contents/MacOS
macdeployqt osx/kafkaui.app -appstore-compliant -executable=osx/kafkaui.app/Contents/MacOS/kafkaui -qmldir=/usr/local/opt/qt/share/qt/qml -dmg
