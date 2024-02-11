#ifndef __QUEUE__H_
#define __QUEUE__H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Queue.h"
#include <assert.h>

#define MAX_SIZE 16


typedef struct {

    int queue[MAX_SIZE];
	int head;
	int tail;
	unsigned int size;

} Queue;


Queue *createQueue();

Queue *push(Queue *q, int v);

Queue *pop(Queue *q);

bool isEmpty (const Queue *q);

int getHead(Queue *q);

int getTail(Queue *q);

int getSize(Queue *q);

void printQueue(Queue *q);

void freeQueue(Queue **q);

#endif
