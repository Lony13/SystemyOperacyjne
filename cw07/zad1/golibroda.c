//
// Created by wojciech on 01.05.17.
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "golibroda.h"

key_t fifoKey;
int shmID;
int semID;
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
    char *path = getenv("HOME");
    fifoKey = ftok(path, PROJECT_ID);
    shmID = shmget(fifoKey, 1024, IPC_CREAT | 0660);
    fifo = shmat(shmID, NULL, 0);
}

void createSemafors(){
    semID = semget(fifoKey, 1, IPC_CREAT | 0660);
    semctl(semID, 0, SETVAL, 1);
}

void startWork(){
    struct sembuf sbuf;
    sbuf.sem_num = 0;
    sbuf.sem_flg = SEM_UNDO;

    while(1) {
        if(isEmpty(fifo) == 1){
            fifo[1] = 0;                //status golibrody -> wolny/spi
            printf("Nie ma klientow wiec ide spac");
            printTime();
            while(isEmpty(fifo) == 1);  //czekanie na pojawienie się klienta w poczekalni
        }

        sbuf.sem_op = -1;
        semop(semID, &sbuf, 1);

        fifo[1] = popFifo(fifo);

        sbuf.sem_op = 1;
        semop(semID, &sbuf, 1);

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

    shmdt(fifo);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID);

    exit(0);
}

void signalHandler(int signum){
    printf("\nOtrzymalem sygnal SIGINT\n");
    exit(0);
}