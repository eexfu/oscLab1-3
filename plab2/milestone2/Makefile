all: main.c sensor_db.h sensor_db.c logger.h logger.c
	gcc -g -Wall main.c sensor_db.c logger.c -o main.out

run: all
	./main.out

memtest: all
	valgrind -s --leak-check=full --show-leak-kinds=all ./main.out

debug:	all
	gdb main.out

cppcheck:
	cppcheck --enable=all .

zip:
	zip milestone2.zip *.c *.h

clean:
	rm -r *.out