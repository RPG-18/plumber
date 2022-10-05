#!/bin/zsh

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64" -G Ninja
cmake --build build --config Release

mkdir -p build/plumber.app/Contents/Resources/qml/Qt/labs
cp -R $HOME/Qt/6.3.2/macos/qml/Qt/labs/platform build/plumber.app/Contents/Resources/qml/Qt/labs/platform

macdeployqt build/plumber.app \
  -appstore-compliant \
  -executable=build/plumber.app/Contents/MacOS/plumber \
  -libpath=$HOME/Qt/6.3.2/macos/lib \
  -qmldir=$HOME/Qt/6.3.2/macos/qml \

echo "Ad hoc sign"
codesign --force -s - build/plumber.app/Contents/Frameworks/*.dylib

echo "Try sign bundle"
codesign -f -o runtime --timestamp -s FC0E864A8F1C98EA5FB509624EA36E4E74E50474 --deep build/plumber.app

echo "Create image"
hdiutil create build/plumber.dmg -srcfolder build/plumber.app -format UDZO -volname plumber

echo "Sign dmg"
codesign --force -s FC0E864A8F1C98EA5FB509624EA36E4E74E50474 build/plumber.dmg