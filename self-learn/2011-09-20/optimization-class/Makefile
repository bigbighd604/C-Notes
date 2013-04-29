
#CC=gcc -m64 -pg
CC=gcc -m64

LDFLAGS=`sdl-config --libs`
CFLAGS=`sdl-config --cflags` -O1 -g -Wall

all: optimization-class

%.o: %.asm
	nasm -felf64 -o $@ $^

clean:
	$(RM) *.o optimization-class core gmon.*

optimization-class: main.o extra.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $^
