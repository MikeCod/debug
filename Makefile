dst := /usr/local/include
src := ./src
build := build
env := $(shell sed -E 's/^(.+)/\-D\1/g' .env)


all: build-c build-c++ build-c++-asm

i: install
install:
	@sudo mkdir -p ${dst}/debug/
	@sudo cp -vr ${src}/* ${dst}/debug/

u: uninstall
uninstall:
	@sudo rm -vr ${dst}/debug/

build-c:
	@mkdir -p ${build}
	@gcc example.c -o ${build}/example-c -DDEBUG -Wall ${env}

build-c++:
	@mkdir -p ${build}
	@g++ example.cpp -o ${build}/example-cpp -std=c++20 -DDEBUG ${env}

build-c++-asm:
	@mkdir -p ${build}
	@g++ example.cpp -std=c++20 -fverbose-asm -masm=intel -S -o ${build}/example-cpp.asm

clean:
	@rm -rf build