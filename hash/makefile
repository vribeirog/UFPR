CC = gcc
CFLAGS = -g -Wall -std=c90

programa : myht.o hash.o
	$(CC) myht.o hash.o -o myht

main.o : myht.c
	$(CC) $(CFLAGS) -c myht.c

hash.o : hash.c
	$(CC) $(CFLAGS) -c hash.c

clean : 
	rm *.o myht
