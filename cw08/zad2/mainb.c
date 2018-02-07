#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <pthread.h>
#include <syscall.h>

#define RECORD_SIZE 1024

typedef struct threadData{
    pthread_t* threadTab;
}thread_data_t;

int file;
int recordsToRead;
int numberOfThreads;
char* keyWord;
pthread_mutex_t mutex;

void* readFile(void* arg);
void* divByZero(void* arg);

int main(int argc, char** argv) {
    if(argc != 5){
        printf("Niewlasciwa liczba argumentow\n");
        exit(0);
    }

    numberOfThreads = atoi(argv[1]);
    char* fileName = argv[2];
    recordsToRead = atoi(argv[3]);
    keyWord = argv[4];

    file = open(fileName, O_RDONLY);
    if(file == -1){
        printf("Blad podczas otwarcia pliku\n");
        exit(0);
    }

    if(pthread_mutex_init(&mutex, NULL) != 0){
        printf("Blad podczas inicjalizacji mutexu\n");
        exit(0);
    }

    thread_data_t data;
    data.threadTab = malloc(sizeof(pthread_t) * numberOfThreads);

    pthread_t threadID;
    int i;
    for(i = 0; i < numberOfThreads; i++){
        pthread_create(&threadID, NULL, &readFile, &data);
        data.threadTab[i] = threadID;
    }

    //watek ktory bedzie dzielil przez 0
    pthread_create(&threadID, NULL, &divByZero, &data);

    for(i = 0; i < numberOfThreads; i++)
        pthread_join(data.threadTab[i], NULL);
    printf("Koniec macierzystego\n");

    close(file);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* divByZero(void* arg){
    int x = 20;
    int y = 0;
    x = x / y;
    return NULL;
}

void* readFile(void *data){
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    thread_data_t* thData = data;
    char bufor[RECORD_SIZE];
    char recordID[11];
    recordID[10] = '\0';
    int i, j;

    while(1) {
        pthread_mutex_lock(&mutex);
        for(i = 0; i < recordsToRead; i++) {
            if (read(file, &bufor, sizeof(bufor)) == 0) {
                printf("Nic nie bylo do przeczytania %li\n", syscall(SYS_gettid));
                pthread_mutex_unlock(&mutex);
                return NULL;
            }

            if (strstr(bufor, keyWord) != NULL) {
                for(j = 0; j < 10; j++)
                    recordID[j] = bufor[j];
                printf("Watek %li znalazl kluczowe slowo w rekordzie o id: %s\n", syscall(SYS_gettid), recordID);
                for(j = 0; j < numberOfThreads; j++) {
                    if (pthread_self() != thData->threadTab[j]) {
                        pthread_cancel(thData->threadTab[j]);
                    }
                }
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(1);
    }
    return NULL;
}