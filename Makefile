CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations

all: tis

run: tis
	./tis

tis: src/tis.c build/program.o build/node.o build/util.o build/input_code.o build/node_list.o build/output.o
	$(CC) $(CFLAGS) -o tis src/tis.c build/program.o build/node.o build/util.o build/input_code.o build/node_list.o build/output.o -lncurses -lm

build/program.o: src/program.c src/program.h src/node.h src/node_list.h
	$(CC) $(CFLAGS) -c src/program.c -o build/program.o

build/node.o: src/node.c src/node.h
	$(CC) $(CFLAGS) -c src/node.c -o build/node.o

build/util.o: src/util.h src/util.c
	$(CC) $(CFLAGS) -c src/util.c -o build/util.o

build/input_code.o: src/input_code.h src/input_code.c
	$(CC) $(CFLAGS) -c src/input_code.c -o build/input_code.o

build/node_list.o: src/node_list.h src/node_list.c
	$(CC) $(CFLAGS) -c src/node_list.c -o build/node_list.o

build/output.o: src/output.h src/output.c
	$(CC) $(CFLAGS) -c src/output.c -o build/output.o

valgrind: tis
	valgrind ./tis

clean:
	rm build/*.o
	rm -f tis
