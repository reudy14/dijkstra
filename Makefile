EXEC=test.o
CC=clang
CFLAGS= -Wall -std=c99 -lm

all:
	$(CC) -c -g $(CFLAGS) *.c 
	$(CC) $(CFLAGS) test.c *.o -o test.o

run: all
	./test.o

val: all
	valgrind --leak-check=full ./test.o

gdb: all
	gdb ./test.o

test: all
	../bin/tdijkstra-lnx -h -n 1000 -g graph.txt -p solutionFile.txt
	./test.o
