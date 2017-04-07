CC=gcc
CFLAGS=-Wall -fPIC -c
all: library.so library.a static shared dynamic

library.so:
	export LD_LIBRARY_PATH=./zad3a
	$(CC) $(CFLAGS) library.c -o library.o
	$(CC) -shared -o library.so library.o

library.a:
	$(CC) -c library.c -o library.o
	ar rs library.a library.o

static:
	$(CC) -o staticMain.o -I . -c main.c
	$(CC) -o staticMain staticMain.o library.a
	echo "Statyczna:" >> results.txt
	./staticMain >> results.txt

shared:
	$(CC) -o sharedMain.o -I . -c main.c
	$(CC)   -o sharedMain sharedMain.o library.so
	echo "Dzielona:" >> results.txt
	./sharedMain >> results.txt

dynamic:
	$(CC) -rdynamic -o dynamicMain dynamicMain.c -ldl -I .
	echo "Dynamiczna:" >> results.txt
	./dynamicMain >> results.txt

clear:
	rm staticMain.o sharedMain.o staticMain sharedMain results.txt
