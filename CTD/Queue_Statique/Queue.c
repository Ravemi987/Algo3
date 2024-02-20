#include "Queue.h"
#include <assert.h>

#define MAX_SIZE 16


Queue *createQueue() {
	Queue *q = malloc(sizeof(Queue));
	if (q == NULL) return NULL;
	q->head = 0;
    q->tail = -1;
	q->size = 0;

	return(q);
}

Queue *push(Queue *q, int v) {
	assert(q->size < MAX_SIZE);
	q->tail = (q->tail + 1) % MAX_SIZE;
	q->queue[q->tail] = v;
	++(q->size);
	return (q);
}

Queue *pop(Queue *q) {
	assert(!isEmpty(q));
	q->head = (q->head + 1) % MAX_SIZE;
	--(q->size);
	return (q);
}

//Opérateurs d'état du TAD Queue
bool isEmpty (const Queue *q){
	return q->size == 0;
}

int getHead(Queue *q) {
	assert(!isEmpty(q));
	return q->queue[q->head];
}

int getTail(Queue *q) {
	assert(!isEmpty(q));
	return q->queue[q->tail];
}

int getSize(Queue *q) {
	return q->size;
}

void printQueue(Queue *q) {
	for (int i = q->head; i <= q->tail; i++) {
		printf("%d ", q->queue[i]);
	}

	printf("\n");
}

void freeQueue(Queue **q) {
	if (*q != NULL) {
		free(*q);
		*q = NULL;
	}
}
