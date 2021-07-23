$Env:Path += ";C:\Qt\6.1.2\msvc2019_64\bin"

& cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
& cmake --build build --config Release
& C:/Qt/6.1.2/msvc2019_64/bin/windeployqt.exe --qmldir C:/Qt/6.1.2/msvc2019_64/qml build/Release
Rename-Item  -Path build/Release -NewName kafkaui
Compress-Archive -Path build/kafkaui -DestinationPath kafkaui_win64.zip -CompressionLevel Optimal