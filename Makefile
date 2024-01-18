dst := /usr/local/include
src := ./src
file := example.c
env := $(shell sed -E 's/^(.+)/\-D\1/g' .env)


all: build

i: install
install:
	@sudo mkdir -p ${dst}/debug/
	@sudo cp -vr ${src}/* ${dst}/debug/

u: uninstall
uninstall:
	@sudo rm -vr ${dst}/debug/

build:
	@gcc ${file} -DDEBUG -Wall ${env}