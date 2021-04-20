CC=gcc
FLAGS=-I.

.PHONY: all clean

all: server client

server: MultModulo.o MultModulo.h
	$(CC) MultModulo.o server.c -o server -lpthread $(FLAGS)

client: MultModulo.o MultModulo.h
	$(CC) MultModulo.o client.c -o client $(FLAGS)

MultModulo.o: MultModulo.h
	$(CC) -c MultModulo.c -o MultModulo.o $(FLAGS)

clean:
	rm server client ports_list.txt MultModulo.o