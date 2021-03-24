CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: parallel_min_max zombies process_memory

parallel_min_max: utils.o find_min_max.o utils.h find_min_max.h
	$(CC) parallel_min_max.c utils.o find_min_max.o -o parallel_min_max $(CFLAGS)

utils.o: utils.h
	$(CC) -c utils.c -o utils.o $(CFLAGS)

find_min_max.o: find_min_max.h utils.h
	$(CC) -c find_min_max.c -o find_min_max.o $(CFLAGS)

zombies: 
	$(CC) zombies.c -o zombies

process_memory:
	$(CC) process_memory.c -o process_memory
 
clean:
	rm parallel_min_max utils.o find_min_max.o zombies process_memory