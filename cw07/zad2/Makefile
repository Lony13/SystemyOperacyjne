all: golibroda klient

golibroda:
	gcc -c golibroda.c
	gcc -o golibroda golibroda.o -lrt -lpthread

klient:
	gcc -c klient.c
	gcc -o klient klient.o -lrt -lpthread

clear:
	rm golibroda klient golibroda.o klient.o
