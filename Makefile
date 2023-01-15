EXEC=test.o
CC=clang
CFLAGS= -Wall -Wnewline-eof -std=c99 -O3 -lm

all:
	$(CC) -c -g $(CFLAGS) *.c 
	$(CC) $(CFLAGS) test.c *.o -o test.o

run: all
	./test.o

val: all
	valgrind --leak-check=full ./test.o

gdb: all
	gdb ./test.o

n := 10
test: all
	./tdijkstra-lnx -h -n $(n) -g graph.txt
	./test.o
	./tdijkstra-lnx -t graph.txt solution.txt


CFILES := $(wildcard *.c)
CFILES := $(filter-out test.c, $(CFILES))
zip: all
	zip -r a.zip $(CFILES) *h
