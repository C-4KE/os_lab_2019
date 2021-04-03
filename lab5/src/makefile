CC=gcc
FLAGS=-lpthread -I.

.PHONY: all clean

all: mutex mod_factorial

mutex:
	$(CC) mutex.c -o mutex $(FLAGS)

mod_factorial:
	$(CC) mod_factorial.c -o mod_factorial $(FLAGS)

clean:
	rm mutex mod_factorial