#!/bin/zsh

export PATH="/usr/local/bin:$PATH"
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

cp build/kafkaui osx/kafkaui.app/Contents/MacOS
install_name_tool -add_rpath "@executable_path/../Frameworks" osx/kafkaui.app/Contents/MacOS/kafkaui
macdeployqt osx/kafkaui.app -appstore-compliant -qmldir=/usr/local/opt/qt/share/qt/qml -dmg > /dev/null 2>&1
