CC=gcc
CFLAGS=-pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations

all: tis

run: tis
	./tis

tis: tis.c program.o
	$(CC) $(CFLAGS) -o tis tis.c program.o

program.o: program.c program.h
	$(CC) $(CFLAGS) -c program.c

valgrind: tis
	valgrind ./tis

clean:
	rm -f tis
