#include "Stack.h"

int main(void) {
    printf("Creation d'une Pile: ");
    Stack *s = createStack();
    printf("Ok\n");

    for (int i = 1; i < 4; i++) {
        push(s, i);
        printf("Push \"%d\" -> ", i);
        printStack(s);
    }

    printf("Vide ? %d \n", isEmpty(s));
    printf("==================\n");

    for (int i = 0; i < 2; i++) {
        printf("Pop (%d) -> ", get(s));
        pop(s);
        printStack(s);
        printf("top : %d\n", top(s));
        printf("tete : %d\n", get(s));
        printf("Vide ? %d \n", isEmpty(s));
        printf("==================\n");
    }

    printf("Pop (%d) -> ", get(s));
    pop(s);
    printStack(s);
    printf("Vide ? %d \n", isEmpty(s));

    printf("Liberation de la Pile:\n");
    freeStack(&s);
    freeStack(&s);

    if (s == NULL) {
        printf("Ok\n");
    } else {
        printf("Erreur de liberation de la mémoire.\n");
    }

    return 0;
}
