NASM=nasm
CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lX11 -lm

all: geometric

geometric: geometric.o visualization.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

geometric.o: src/geometric.c
	$(CC) $(CFLAGS) -c -o $@ $<

visualization.o: src/visualization.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o geometric 