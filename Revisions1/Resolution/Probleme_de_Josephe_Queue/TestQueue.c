#include "Queue.h"

Queue *represent(int n) {
    Queue *q = createQueue();
    for (int i = 1; i <= n; i ++)
        q = push(q, i);
    return q;
}

int solve(Queue *q, int k) {
    int last, tmp;
    while (!isEmpty(q)) {
        printQueue(q);
        for (int i = 1; i < k; i++) {
            tmp = getHead(q);
            q = pop(q);
            q = push(q, tmp);
        }
        last = getHead(q);
        q = pop(q);
    }
    return last;
}

int main(void) {
    Queue *q = represent(8);
    printf("Solution : %d\n", solve(q, 3));

    printf("Liberation de la File:\n");
    freeQueue(&q);

    if (q == NULL) {
        printf("Ok\n");
    } else {
        printf("Erreur de liberation de la mémoire.\n");
    }

    return 0;
}
