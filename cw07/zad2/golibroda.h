//
// Created by wojciech on 01.05.17.
//

#ifndef LAB7_GOLIBRODA_H
#define LAB7_GOLIBRODA_H

static const int PROJECT_ID = 20;

void exitGolibroda();
void signalHandler(int signum);
void createFifo(int N);
void createSemafors();
void startWork();
void printTime();
void prepareFifo(int N);
int isEmpty(int* fifo);
int isFull(int* fifo);
int popFifo(int* fifo);

#endif //LAB7_GOLIBRODA_H
