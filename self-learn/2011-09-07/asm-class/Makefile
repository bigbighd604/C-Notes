
CC=gcc -m32

CCFLAGS=-O2 -g -Wall

all: asm-lab

%.o: %.asm
	nasm -felf -o $@ $^

clean:
	$(RM) *.o asm-lab

asm-lab: main.o extra.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $^
