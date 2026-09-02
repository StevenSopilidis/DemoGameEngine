cmake -S . -B build-debug -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build build-debug --parallel 4