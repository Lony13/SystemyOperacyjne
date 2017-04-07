CC=gcc
CFLAGS=-Wall -fPIC -c
all: library.so library.a

library.so:
	$(CC) $(CFLAGS) library.c -o library.o
	$(CC) -shared -o library.so library.o

library.a:
	$(CC) -c library.c -o library.o
	ar rs library.a library.o