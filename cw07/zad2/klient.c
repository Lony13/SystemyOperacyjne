//
// Created by wojciech on 01.05.17.
//

#include <wait.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "klient.h"

key_t fifoKey;
int shmID;
sem_t* semID;
int* fifo;
int haircut;

int main(int argc, char** argv){
    atexit(exitKlient);
    signal(SIGUSR1, afterHaircut);

    if(argc != 3){
        printf("Niewlasciwa liczba argumentow\n");
        return 1;
    }

    int numberOfClients = atoi(argv[1]);
    int numberOfHaircuts = atoi(argv[2]);

    getFifo();
    getSemafors();

    int i;
    for(i = 0; i < numberOfClients; i++){
        pid_t pid = fork();
        if(pid == 0){
            goHairdresser(numberOfHaircuts);
        }
    }

    for(i = 0; i < numberOfClients; i++){
        int status;
        wait(&status);
    }

    return 0;
}

void goHairdresser(int numberOfHaircuts){
    printf("Klient %d rozpoczal prace\n", getpid());

    int j;
    for(j = 0; j < numberOfHaircuts; j++){
        haircut = 0;            //status obciecia wlosow

        sem_wait(semID);

        if(fifo[1] == 0 && isEmpty(fifo))   //czy kolejka jest pusta i golibroda spi
            getHaircut(fifo, getpid());
        else if(!isFull(fifo))              //czy jest miejsce w kolejce
            sitAndWait(fifo, getpid());
        else{
            printf("Nie ma wolnych miejsc w poczeklani :( Pid: %d", getpid());
            printTime();
            haircut = 1;
            j--;
        }

        sem_post(semID);

        while(haircut != 1);
        printf("Zostalem ostrzyzony juz %d razy. Pid: %d\n", j+1, getpid());
    }

    printf("Koniec strzyzen. Wychodze z zakladu: %d", getpid());
    printTime();
    exit(0);
}

void getHaircut(int* fifo, int pid){
    pushFifo(fifo, pid);
    printf("Obudzilem golibrode %d", pid);
    printTime();
}

void getFifo(){
    shmID = shm_open("/shm", O_RDWR, 0660);
    ftruncate(shmID, 1024);
    fifo = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
}

void getSemafors(){
    if((semID = sem_open("/fifoo", 0)) == SEM_FAILED){
        printf("Error podczas pobierania semaforow\n");
        exit(0);
    }
}

void printTime(){
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    printf("  Czas: %ld\n", time.tv_sec*1000000 + time.tv_nsec/1000);
}

void sitAndWait(int* fifo, int pid){
    pushFifo(fifo, pid);
    printf("Siadam w poczekalni %d", pid);
    printTime();
}

int isEmpty(int* fifo){
    if(fifo[0] != 0) return 0;
    return 1;
}

int isFull(int* fifo){
    if(fifo[0] == fifo[2]) return 1;
    return 0;
}

void pushFifo(int* fifo, int clientPid){
    fifo[0]++;
    fifo[fifo[0] + 3] = clientPid;
}

void afterHaircut(int signum){
    haircut = 1;
}

void exitKlient(){
    printf("Klient konczy prace %d\n", getpid());
    munmap(fifo, 1024);
    sem_close(semID);
    exit(0);
}