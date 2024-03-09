#include "Stack.h"

int main(void) {
    Stack *s = createStack();
    Node *n = createNode(5);

	s = push(s, createNode(4));
	s = push(s, createNode(3));
	s = push(s, createNode(17));
	s = push(s, createNode(48));
	s = push(s, createNode(12));
	s = push(s, createNode(24));
	s = push(s, createNode(7));

    s = push(s, n);
    printStack(s);
    printf("%d\n", getSize(s));

    printf("Liberation de la Pile:\n");
    freeStack(&s);

    if (s == NULL) {
        printf("Ok\n");
    } else {
        printf("Erreur de liberation de la mémoire.\n");
    }

    return 0;
}
