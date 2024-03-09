#include "Queue.h"
#include <stdlib.h>
#include <assert.h>


struct s_node {
    int value;
    struct s_node *next;
};

struct s_queue {
    struct s_node *head;
    struct s_node *tail;
    unsigned int size;
};


Queue *createQueue() {
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

Queue *push(Queue *q, int e) {
    Node **insert_at = (q->size ? &(q->tail->next) : &(q->head));
    Node *new = malloc(sizeof(Node));
    new->value = e;
    new->next = NULL;
    *insert_at = new;
    q->tail = new;
    ++(q->size);
    return q;
}

bool isEmpty(Queue *q) {
    return q->size == 0;
}

Queue *pop(Queue *q) {
    assert(!isEmpty(q));
    Node *toDelete = q->head;
    q->head = q->head->next;
    free(toDelete);
    toDelete = NULL;
    --(q->size);
    return q;
}

int getSize(Queue *q) {
    return q->size;
}

int getHead(Queue *q) {
    assert(!isEmpty(q));
    return q->head->value;
}

int getTail(Queue *q) {
    assert(!isEmpty(q));
    return q->tail->value;
}

void printQueue(Queue *q) {
    for (Node *e = q->head; e != NULL; e = e->next) {
        printf("%d ", e->value);
    }
    printf("\n");
}

void freeQueue(Queue **q) {
    if (*q != NULL) {
        Node *toDelete;
        Node *current = (*q)->head;

        while (current != NULL) {
            toDelete = current;
            current = current->next;
            free(toDelete);
            toDelete = NULL;
        }

        free(*q);
        *q = NULL;
    }
}