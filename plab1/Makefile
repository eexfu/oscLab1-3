
all: main.c datamgr.c datamgr.h config.h lib/dplist.c lib/dplist.h
	gcc -g main.c datamgr.c lib/dplist.c lib/dplist.h -o main.out

gen: config.h file_creator.c 
	gcc -g file_creator.c config.h -o gen.out

gendebug: config.h file_creator.c
	gcc -g file_creator.c config.h -o gendebug.out -DDEBUG

memtest:
	valgrind -s --leak-check=full --show-leak-kinds=all ./main.out

debug:
	gdb main.out

run:
	./main.out

runGendebug:
	./gendebug.out

clean:
	rm -r *.out