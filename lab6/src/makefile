CC=gcc
FLAGS=-I.

.PHONY: all clean

all: server client

server: 
	$(CC) server.c -o server -lpthread $(FLAGS)

client:
	$(CC) client.c -o client $(FLAGS)

clean:
	rm server client ports_list.txt