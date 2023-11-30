zip:
	zip milestone3.zip *.c *.h

all: main.c config.h sbuffer.h sbuffer.c
	gcc -g -Wall main.c sbuffer.c -o main.out

run: all
	./main.out

debug:
	gdb main.out

memtest:
	valgrind -s --leak-check=full --show-leak-kinds=all ./main.out

test:
	gcc *.c -o main.out -lpthread

clean:
	rm -r *.out *.csv