CC=gcc
FLAGS=-lpthread -I.

.PHONY: all clean

all: mutex mod_factorial deadlock

mutex:
	$(CC) mutex.c -o mutex $(FLAGS)

mod_factorial:
	$(CC) mod_factorial.c -o mod_factorial $(FLAGS)

deadlock:
	$(CC) deadlock.c -o deadlock $(FLAGS)

clean:
	rm mutex mod_factorial deadlock