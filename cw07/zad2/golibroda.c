//
// Created by wojciech on 01.05.17.
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "golibroda.h"

key_t fifoKey;
int shmID;
sem_t* semID;
int* fifo;

int main(int argc, char** argv){
    atexit(exitGolibroda);
    signal(SIGINT, signalHandler);

    if(argc != 2){
        printf("Niewlasciwa liczba argumentow\n");
        return 1;
    }

    int N = atoi(argv[1]);

    createFifo(N);
    createSemafors();
    prepareFifo(N);
    startWork();

    return 0;
}

void createFifo(int N){
    shmID = shm_open("/shm", O_CREAT | O_RDWR, 0660);
    ftruncate(shmID, 1024);
    fifo = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
}

void createSemafors(){
    semID = sem_open("/fifoo", O_CREAT, 0660, 1);
}

void startWork(){
    while(1) {
        if(isEmpty(fifo) == 1){
            fifo[1] = 0;                //status golibrody -> wolny/spi
            printf("Nie ma klientow wiec ide spac");
            printTime();
            while(isEmpty(fifo) == 1);  //czekanie na pojawienie się klienta w poczekalni
        }

        sem_wait(semID);
        fifo[1] = popFifo(fifo);
        sem_post(semID);

        printf("Strzyzenie klienta: %d", fifo[1]);
        printTime();
        printf("Klient: %d ostrzyzony", fifo[1]);
        printTime();
        kill(fifo[1], SIGUSR1);
    }
}

void prepareFifo(int N){
    fifo[0] = 0;        //liczba osob w kolejce
    fifo[1] = 0;        //status golibrody -> 0 wolny(spi), != 0 pid siedzacego
    fifo[2] = N;        //liczba miejsc w kolejce
    fifo[3] = getpid(); //pid golibrody
}

void printTime(){
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    printf(" Czas: %ld\n", time.tv_sec*1000000 + time.tv_nsec/1000);
}

int isEmpty(int* fifo){
    if(fifo[0] == 0) return 1;
    return 0;
}

int isFull(int* fifo){
    if(fifo[0] == fifo[2]) return 1;
    return 0;
}

int popFifo(int* fifo){
    if(isEmpty(fifo) == 1) return -1;
    int clientPid = fifo[4];
    int rest = fifo[0];
    fifo[0]--;

    int i;
    for(i = 4; i < rest + 4; i++)
        fifo[i] = fifo[i+1];
    return clientPid;
}

void exitGolibroda(){
    printf("Golibroda konczy prace\n");

    munmap(fifo, 1024);
    shm_unlink("/shm");
    sem_close(semID);
    sem_unlink("/fifoo");

    exit(0);
}

void signalHandler(int signum){
    printf("\nOtrzymalem sygnal SIGINT\n");
    exit(0);
}