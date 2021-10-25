find . \( -iname "*.h" -o -iname "*.cpp" \) -not -path "./3rdparty/*" -not -path "./cmake-build-debug/*" -not -path "./cmake-build-release/*"  | xargs clang-format -i
