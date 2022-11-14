# calls cmake to build but copies over result for testing
.PHONY: all test tar clean

SOURCES = $(shell find ./src/ -name '*.cpp')
INCLUDE = $(shell find ./include/ -name '*.h') 
TESTS = $(shell find ./tests/*) 

all: feedreader

feedreader:
	mkdir -p ./build
	cd ./build && cmake .. && make
	cp ./build/feedreader ./feedreader

test: feedreader
	cd ./build/tests && ctest

tar: clean
	tar -cf xkocic02.tar Makefile CMakeLists.txt LICENSE.txt README.md $(SOURCES) $(INCLUDE) $(TESTS)

clean:
	rm -r -f out
	rm -r -f build
	rm -r -f feedreader
	rm -f xkocic02.tar
