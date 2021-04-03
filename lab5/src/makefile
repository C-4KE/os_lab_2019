CC=gcc
FLAGS=-I.

.PHONY: all clean

all: mutex

mutex:
	$(CC) mutex.c -o mutex -lpthread $(FLAGS)

clean:
	rm mutex