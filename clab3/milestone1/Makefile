
ourtest : dplist.c ourtest.c
	mkdir -p build
	gcc dplist.c ourtest.c -o build/dplist_test -Wall -g
	./build/dplist_test

randomtest : dplist.c randomtest.c
	mkdir -p build
	gcc dplist.c randomtest.c -o build/dplist_test -Wall -g
	./build/dplist_test

all : dplist.c dplist_test.c
	mkdir -p build
	gcc dplist.c dplist_test.c -o build/dplist_test -Wall -g
	./build/dplist_test

memtest:
	valgrind --leak-check=yes ./build/dplist_test

debug:
	gdb ./build/dplist_test

cppcheck:
	cppcheck --enable=all .

clean:
	rm -r build

zip:
	zip milestone1.zip *.c *.h
