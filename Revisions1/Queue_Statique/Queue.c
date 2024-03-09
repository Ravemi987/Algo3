#include "Queue.h"
#include <stdlib.h>
#include <assert.h>


struct s_queue {
    int queue[MAX_SIZE];
    int head;
    int tail;
    unsigned int size;
};


Queue *createQueue() {
    Queue *q = malloc(sizeof(Queue));
    q->head = 0;
    q->tail = -1;
    q->size = 0;
    return q;
}

Queue *push(Queue *q, int e) {
    assert(q->size < MAX_SIZE);
    q->tail = (q->tail + 1) % MAX_SIZE;
    q->queue[q->tail] = e;
    ++(q->size);
    return q;
}

bool isEmpty(Queue *q) {
    return q->size == 0;
}

Queue *pop(Queue *q) {
    assert(!isEmpty(q));
    q->head = (q->head + 1) % MAX_SIZE;
    --(q->size);
    return q;
}

int getSize(Queue *q) {
    return q->size;
}

int getHead(Queue *q) {
    assert(!isEmpty(q));
    return q->queue[q->head];
}

int getTail(Queue *q) {
    assert(!isEmpty(q));
    return q->queue[q->tail];
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