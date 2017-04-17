all: slave master

slave:
	gcc slave.c -o slave -lm

master:
	gcc -c master.c
	gcc -o master master.o

clear:
	rm slave master master.o mkfifo data
