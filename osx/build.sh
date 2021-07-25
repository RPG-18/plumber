#!/bin/zsh

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

cp build/kafkaui osx/kafkaui.app/Contents/MacOS
macdeployqt osx/kafkaui.app -appstore-compliant -qmldir=/Users/dmitry/Qt/6.1.2/macos/qml -dmg