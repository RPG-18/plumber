$Env:Path += ";C:\Qt\6.2.2\msvc2019_64\bin"
$Env:VCINSTALLDIR = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC"

& cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
& cmake --build build --config Release
& C:/Qt/6.2.2/msvc2019_64/bin/windeployqt.exe --qmldir C:/Qt/6.2.2/msvc2019_64/qml build/Release
New-Item -ItemType Directory -Force -Path build/Release/Qt/labs/platform
Copy-Item -Path C:/Qt/6.2.2/msvc2019_64/qml/Qt/labs/platform/* -Destination build/Release/Qt/labs/platform/ -Recurse
Remove-Item build/Release/Qt/labs/platform -Include *.pdb
Rename-Item  -Path build/Release -NewName plumber
Compress-Archive -Path build/plumber -DestinationPath plumber_win64.zip -CompressionLevel Optimal