CC=gcc
CFLAGS=-O2

filter: filter.o main.o
	gcc -o filter filter.o main.o

filter.o: filter.h filter.c
	gcc -o filter.o -c filter.c

main.o: filter.h main.c
	gcc -o main.o -c main.c

clean:
	rm  *.o filter
