/* Garde pour éviter l'inclusion multiple */
#ifndef __QUEUE_H__
#define __QEUE_H__

/* Inclusion des dépendances */
#include <stdbool.h>
#include <stdio.h>

/* Déclaration des types */
typedef struct s_node Node;
typedef struct s_queue Queue;

/* Déclaration des fonctions */
Queue *createQueue();
Queue *push(Queue *q, int e);
bool isEmpty(Queue *q);
Queue *pop(Queue *q);
int getSize(Queue *q);
int getHead(Queue *q);
int getTail(Queue *q);
void printQueue(Queue *q);
void freeQueue(Queue **q);

#endif
