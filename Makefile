# makefile for move generator

all: main.c
	gcc -o perft main.c board.c data.c parsefen.c

clean:
	rm perft
