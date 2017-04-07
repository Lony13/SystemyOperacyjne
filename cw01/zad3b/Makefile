CC=gcc
CFLAGS=-Wall -fPIC -c
all: library.so library.a staticMain sharedMain dynamicMain clear

library.so:
	export LD_LIBRARY_PATH=./zad3b
	$(CC) $(CFLAGS) library.c -o library.o
	$(CC) -shared -o library.so library.o

library.a:
	$(CC) -c library.c -o library.o
	ar rs library.a library.o

staticMain:
	$(CC) -o staticMainO0 -I . main.c library.a -O0
	echo "Statyczne O0:" >> results.txt
	(time ./staticMainO0) &>> results.txt
	$(CC) -o staticMainO1 -I . main.c library.a -O1
	echo "Statyczne O1:" >> results.txt
	(time ./staticMainO1) &>> results.txt
	$(CC) -o staticMainO2 -I . main.c library.a -O2
	echo "Statyczne O2:" >> results.txt
	(time ./staticMainO2) &>> results.txt
	$(CC) -o staticMainOs -I . main.c library.a -Os
	echo "Statyczne Os:" >> results.txt
	(time ./staticMainOs) &>> results.txt
	$(CC) -o staticMainO3 -I . main.c library.a -O3
	echo "Statyczne O3:" >> results.txt
	(time ./staticMainO3) &>> results.txt
	$(CC) -o staticMainOfast -I . main.c library.a -Ofast
	echo "Statyczne Ofast:" >> results.txt
	(time ./staticMainOfast) &>> results.txt

sharedMain:
	$(CC) -o sharedMainO0 -I . main.c library.so -O0
	echo "Dzielone O0:" >> results.txt
	(time ./sharedMainO0) &>> results.txt
	$(CC) -o sharedMainO1 -I . main.c library.so -O1
	echo "Dzielone O1:" >> results.txt
	(time ./sharedMainO1) &>> results.txt
	$(CC) -o sharedMainO2 -I . main.c library.so -O2
	echo "Dzielone O2:" >> results.txt
	(time ./sharedMainO2) &>> results.txt
	$(CC) -o sharedMainOs -I . main.c library.so -Os
	echo "Dzielone Os:" >> results.txt
	(time ./sharedMainOs) &>> results.txt
	$(CC) -o sharedMainO3 -I . main.c library.so -O3
	echo "Dzielone O3:" >> results.txt
	(time ./sharedMainO3) &>> results.txt
	$(CC) -o sharedMainOfast -I . main.c library.so -Ofast
	echo "Dzielone Ofast:" >> results.txt
	(time ./sharedMainOfast) &>> results.txt

dynamicMain:
	$(CC) -rdynamic -o dynamicMainO0 -I . dynamicMain.c -ldl -O0
	echo "Dynamiczne O0:" >> results.txt
	(time ./dynamicMainO0) &>> results.txt
	$(CC) -rdynamic -o dynamicMainO1 -I . dynamicMain.c -ldl -O1
	echo "Dynamiczne O1:" >> results.txt
	(time ./dynamicMainO1) &>> results.txt
	$(CC) -rdynamic -o dynamicMainO2 -I . dynamicMain.c -ldl -O2
	echo "Dynamiczne O2:" >> results.txt
	(time ./dynamicMainO2) &>> results.txt
	$(CC) -rdynamic -o dynamicMainOs -I . dynamicMain.c -ldl -Os
	echo "Dynamiczne Os:" >> results.txt
	(time ./dynamicMainOs) &>> results.txt
	$(CC) -rdynamic -o dynamicMainO3 -I . dynamicMain.c -ldl -O3
	echo "Dynamiczne O3:" >> results.txt
	(time ./dynamicMainO3) &>> results.txt
	$(CC) -rdynamic -o dynamicMainOfast -I . dynamicMain.c -ldl -Ofast
	echo "Dynamiczne Ofast:" >> results.txt
	(time ./dynamicMainOfast) &>> results.txt

clear:
	rm staticMainO0 staticMainO1
	rm staticMainO2 staticMainOs
	rm staticMainO3 staticMainOfast
	rm sharedMainO0 sharedMainO1
	rm sharedMainO2 sharedMainOs
	rm sharedMainO3 sharedMainOfast
	rm dynamicMainO0 dynamicMainO1
	rm dynamicMainO2 dynamicMainOs
	rm dynamicMainO3 dynamicMainOfast
