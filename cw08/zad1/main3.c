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

    sleep(1);
    printf("Koniec macierzystego\n");

    close(file);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* readFile(void *data){
    pthread_detach(pthread_self());
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

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
                pthread_exit(0);
            }

            if (strstr(bufor, keyWord) != NULL) {
                for(j = 0; j < 10; j++)
                    recordID[j] = bufor[j];
                printf("Watek %li znalazl kluczowe slowo w rekordzie o id: %s\n", syscall(SYS_gettid), recordID);
                pthread_mutex_unlock(&mutex);
                pthread_exit(0);
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(1);
    }
}