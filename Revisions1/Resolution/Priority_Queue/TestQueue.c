#include "Queue.h"

int main(void) {
    printf("Creation d'une File: ");
    Queue *q = createQueue();
    printf("Ok\n");

	q = push(q, 4);
	q = push(q, 3);
	q = push(q, 17);
	q = push(q, 48);
	q = push(q, 12);
	q = push(q, 24);
	q = push(q, 7);

    q = push(q, 5);
    q = push(q, 50);
    printQueue(q);
    q = pop(q);
    printQueue(q);
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
