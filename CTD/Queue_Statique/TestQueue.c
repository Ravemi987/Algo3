#include "Queue.h"

int main(void) {
    printf("Creation d'une File: ");
    Queue *q = createQueue();
    printf("Ok\n");

    for (int i = 1; i < 4; i++) {
        q = push(q, i);
        printf("Push \"%d\" -> ", i);
        printQueue(q);
    }

    printf("Vide ? %d \n", isEmpty(q));
    printf("==================\n");

    for (int i = 0; i < 2; i++) {
        printf("Pop (%d) -> ", getHead(q));
        q = pop(q);
        printQueue(q);
        printf("head : %d\n", getHead(q));
        printf("tail : %d\n", getTail(q));
        printf("size : %d\n", getSize(q));
        printf("Vide ? %d \n", isEmpty(q));
        printf("==================\n");
    }

    printf("Pop (%d) -> ", getHead(q));
    q = pop(q);
    printQueue(q);
    printf("Vide ? %d \n", isEmpty(q));

    printf("Liberation de la File:\n");
    freeQueue(&q);
    freeQueue(&q);

    if (q == NULL) {
        printf("Ok\n");
    } else {
        printf("Erreur de liberation de la mémoire.\n");
    }

    return 0;
}
