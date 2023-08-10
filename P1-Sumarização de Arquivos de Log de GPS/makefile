OBJS = bike.o gps.o
SOURCE = bike.c gps.c
HEADER = bike.h
OUT = gps
CC = gcc
CFLAGS = -g -Wall -std=c99

all: $(OBJS)
	$(CC) $(CFLAGS) gps.o bike.o -lm -o gps

bike.o: bike.c
	$(CC) $(CFLAGS) -c bike.c

gps.o: gps.c
	$(CC) $(CFLAGS) -c gps.c

clean:
	rm *.o 

purge: clean
	rm gps
