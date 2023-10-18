all: date_struct swap_pointers temperature cstring

date_struct: date_struct.c
	gcc -Wall date_struct.c -o date_struct

swap_pointers: swap_pointers.c
	gcc -Wall swap_pointers.c -o swap_pointers

temperature:
	gcc -Wall temperature.c -o temperature

cstring:
	gcc -g -w -Wall cstring.c -o cstring

run_date_struct: date_struct
	./ex2

run_swap_pointers: swap_pointers
	./swap_pointers

run_temperature: temperature
	./temperature

run_cstring: cstring
	./cstring

memtest: all
	valgrind --leak-check=yes ./cstring

clean:
	rm -rf *.o
