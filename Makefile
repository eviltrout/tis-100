CC=gcc
CFLAGS=-pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations

all: tis

run: tis
	./tis

tis: tis.c program.o node.o util.o
	$(CC) $(CFLAGS) -o tis tis.c program.o node.o util.o

program.o: program.c program.h node.h
	$(CC) $(CFLAGS) -c program.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

valgrind: tis
	valgrind ./tis

clean:
	rm *.o
	rm -f tis
