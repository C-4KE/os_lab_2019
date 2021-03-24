CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: parallel_min_max zombies process_memory parallel_sum

parallel_min_max: utils.o find_min_max.o utils.h find_min_max.h
	$(CC) parallel_min_max.c utils.o find_min_max.o -o parallel_min_max $(CFLAGS)

parallel_sum: utils.o evaluate_sum.o
	$(CC) -lpthread parallel_sum.c utils.o evaluate_sum.o -o parallel_sum $(CFLAGS)

utils.o: utils.h
	$(CC) -c utils.c -o utils.o $(CFLAGS)

evaluate_sum.o: evaluate_sum.h
	$(CC) -c evaluate_sum.c -o evaluate_sum.o $(CFLAGS)

find_min_max.o: find_min_max.h utils.h
	$(CC) -c find_min_max.c -o find_min_max.o $(CFLAGS)

zombies: 
	$(CC) zombies.c -o zombies $(CFLAGS)

process_memory:
	$(CC) process_memory.c -o process_memory $(CFLAGS)
 
clean:
	rm parallel_min_max utils.o find_min_max.o zombies process_memory parallel_sum evaluate_sum.o