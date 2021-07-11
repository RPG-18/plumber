find . \( -iname "*.h" -o -iname "*.cpp" \) -not -path "./3rdparty/*" -not -path "./cmake-build-debug/*" | xargs clang-format -i
