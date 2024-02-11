#ifndef __QUEUE__H_
#define __QUEUE__H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_queue Queue;
typedef struct d_queue Maillon;

Maillon *createMaillon();

Queue *createQueue();

Queue *push(Queue *q, int v);

Queue *pop(Queue *q);

bool isEmpty (const Queue *q);

int getHead(Queue *q);

int getTail(Queue *q);

int getSize(Queue *q);

void printQueue(Queue *q);

void freeMaillon(Maillon **m);

void freeQueue(Queue **q);

#endif
