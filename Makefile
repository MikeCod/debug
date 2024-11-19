dst := /usr/local/include
src := ./src
env := $(shell sed -E 's/^(.+)/\-D\1/g' .env)


all: build-c build-c++

i: install
install:
	@sudo mkdir -p ${dst}/debug/
	@sudo cp -vr ${src}/* ${dst}/debug/

u: uninstall
uninstall:
	@sudo rm -vr ${dst}/debug/

build-c:
	@gcc example.c -DDEBUG -Wall ${env}

build-c++:
	@g++ example.cpp -O1 -std=c++20 -DDEBUG -Wall ${env}

clean:
	@rm a.out