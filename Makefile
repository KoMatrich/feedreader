# default target is running the CMake in the subdirectory
.PHONY: all test clean
all:
    mkdir -p build
    cd build && cmake .. && make

test: all
    cd build && ctest

clean:
    rm -r build