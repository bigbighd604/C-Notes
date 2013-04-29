# leczb: One note: you don't have to provice the gcc commands, as
#        the GNU Make utility has its own built-in recipies for C programs.
main: main.o common.o filter.o
	gcc -o main main.o common.o filter.o

main.o: main.c common.h filter.h
	gcc -o main.o -c main.c

common.o: common.c filter.h
	gcc -o common.o -c common.c

filter.o: filter.c common.h
	gcc -o filter.o -c filter.c

basic: basic.c
	gcc -o basic basic.c

full: main.o common.o full_filter.o
	gcc -o full main.o common.o full_filter.o

filter_full.o: full_filter.c common.h
	gcc -o full_filter.o full_filter.c
clean:
	rm *.o main basic full
