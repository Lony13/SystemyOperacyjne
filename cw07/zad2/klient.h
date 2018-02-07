//
// Created by wojciech on 01.05.17.
//

#ifndef LAB7_KLIENT_H
#define LAB7_KLIENT_H

static const int PROJECT_ID = 20;

void goHairdresser(int numberOfHaircuts);
void getFifo();
void getSemafors();
void exitKlient();
void printTime();
int isEmpty(int* fifo);
int isFull(int* fifo);
void pushFifo(int* fifo, int clientPid);
void getHaircut(int* fifo, int pid);
void sitAndWait(int* fifo, int pid);
void afterHaircut(int signum);

#endif //LAB7_KLIENT_H
