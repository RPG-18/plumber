find . \( -iname "*.qml" \) -not -path "./cmake-build-debug/*" | xargs qmlformat -i
