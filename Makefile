P=cube
OBJECTS=chunk.o memory.o debug.o
CFLAGS=-g -Wall -Werror -O3 -std=gnu11
LDLIBS=
CC=gcc

$(P): $(OBJECTS)

clean:
	rm -f *.o
	rm -f cube
