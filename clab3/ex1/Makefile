
all : dplist.c dplist.h program.c
	mkdir -p build
	gcc dplist.c program.c -o build/dplistprogram -std=c11 -Wall -g

run: clean all
	./build/dplistprogram

memtest: clean all
	valgrind --leak-check=yes ./build/dplistprogram

debug: clean all
	gdb ./build/dplistprogram core

clean:
	rm -fr *.o build/*
	rm -r build

