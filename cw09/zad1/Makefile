all: maina mainb

maina:
	gcc -c maina.c
	gcc -o maina maina.o -lrt -lpthread

mainb:
	gcc -c mainb.c
	gcc -o mainb mainb.o -lrt -lpthread

clear:
	rm maina mainb maina.o mainb.o
