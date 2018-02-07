#include <wait.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/msg.h>
#include <pthread.h>
#include <syscall.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>

int tab[200];
int shmID;
int divisor;
int descModeW = 0;
int descModeR = 0;
int numberOfWriters;
int numberOfReaders;
int currentReaders = 0;
int waitingWriters = 0;

sem_t semaphore, x, y;
pthread_t* writersID;
pthread_t* readersID;

void initMemory();
void readMemory();
void exitProgram();
void writeMemory();
void* reader(void* arg);
void* writer(void* arg);
void checkArguments(int argc, char** argv);

int main(int argc, char** argv) {
    atexit(exitProgram);
    srand(time(NULL));

    checkArguments(argc, argv);
    initMemory();

    int i;
    for(i = 0; i < numberOfWriters; i++)
        pthread_create(&writersID[i], NULL, &writer, NULL);

    for(i = 0; i < numberOfReaders; i++)
        pthread_create(&readersID[i], NULL, &reader, NULL);

    for(i = 0; i < numberOfWriters; i++)
        pthread_join(writersID[i], NULL);

    for(i = 0; i < numberOfReaders; i++)
        pthread_join(readersID[i], NULL);

    return 0;
}

void* writer(void* arg){
    usleep(rand()%100000);
    while(1){
        sem_wait(&x);
        waitingWriters++;
        sem_post(&x);

        sem_wait(&semaphore);
        writeMemory();
        sem_post(&semaphore);

        sem_wait(&x);
        waitingWriters--;
        sem_post(&x);
    }
}

void* reader(void* arg){
    usleep(rand()%100000);
    while(1){
        while(waitingWriters != 0){}
        sem_wait(&y);
        if(currentReaders++ == 0) sem_wait(&semaphore);
        sem_post(&y);

        readMemory();

        sem_wait(&y);
        if(--currentReaders == 0) sem_post(&semaphore);
        sem_post(&y);
    }
}

void writeMemory(){
    int numberOfChanges = rand()%50 + 1, i, index, value;
    for(i = 0; i < numberOfChanges; i++){
        index = rand()%200;
        value = rand()%1000;
        tab[index] = value;
        if(descModeW)
            printf("Zmienilem tab[%d] na wartosc %d. Pisarz %d\n", index, value, syscall(SYS_gettid));
    }
    printf("Zmodyfikowalem tablice. Pisarz %d\n", syscall(SYS_gettid));

}

void readMemory(){
    int result = 0, i;
    printf("Czytelnik %d rozpoczyna czytanie\n", syscall(SYS_gettid));
    for(i = 0; i < 200; i++){
        if(tab[i] % divisor == 0){
            result++;
            if(descModeR)
                printf("Tab[%d] = %d jest podzielna przez %d - Czytelnik %d\n", i, tab[i], divisor, syscall(SYS_gettid));
        }
    }
    printf("%d liczb jest podzielnych przez %d. Czytelnik %d\n", result, divisor, syscall(SYS_gettid));
}

void checkArguments(int argc, char** argv){
    if(argc != 4 && argc != 5 && argc != 6){
        printf("Niewlasciwa liczba argumentow\n");
        exit(0);
    }

    numberOfWriters = atoi(argv[1]);
    numberOfReaders = atoi(argv[2]);
    divisor = atoi(argv[3]);
    writersID = malloc(sizeof(pthread_t) * numberOfWriters);
    readersID = malloc(sizeof(pthread_t) * numberOfReaders);

    if(argc == 5 && strcmp(argv[4], "-iw") == 0)
        descModeW = 1;

    if(argc == 5 && strcmp(argv[4], "-ir") == 0)
        descModeR = 1;

    if(argc == 6 && strcmp(argv[4], "-iw") == 0 && strcmp(argv[5], "-ir") == 0){
        descModeR = 1;
        descModeW = 1;
    }

    if(argc == 6 && strcmp(argv[4], "-ir") == 0 && strcmp(argv[5], "-iw") == 0){
        descModeR = 1;
        descModeW = 1;
    }
}

void initMemory(){
    sem_init(&semaphore, 0, 1);
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

    int i;
    for(i = 0; i < 200; i++)
        tab[i] = i;
}

void exitProgram(){\
    sem_destroy(&semaphore);
    exit(0);
}