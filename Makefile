.PHONY: all lib clean

OUT_DIRECTORY := out
SOURCE_DIRECTORY := src

all: lib

lib: $(OUT_DIRECTORY)/lib/arguments.so

$(OUT_DIRECTORY)/lib/arguments.so: $(SOURCE_DIRECTORY)/arguments.cpp | $(OUT_DIRECTORY)/lib/
	g++ -shared -Wall -fPIC --std=c++23 -O3 -o $@ $^

$(OUT_DIRECTORY)/%/:
	mkdir -p $@

clean:
	rm -rf out
