CC=gcc
CFLAGS=-pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations

all: tis

run: tis
	./tis

tis: tis.c program.o node.o util.o input_code.o
	$(CC) $(CFLAGS) -o tis tis.c program.o node.o util.o input_code.o

program.o: program.c program.h node.h
	$(CC) $(CFLAGS) -c program.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

input_code.o: input_code.h input_code.c
	$(CC) $(CFLAGS) -c input_code.c


valgrind: tis
	valgrind ./tis

clean:
	rm *.o
	rm -f tis
