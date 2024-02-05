#include "StaticStack.h"

int main(void) {
    Stack s;
    createStack(&s);
    push(&s, 1);
    push(&s, 2);
    push(&s, 3);
    printStack(&s);
    pop(&s);
    printStack(&s);
    printf("top : %d\n", s.top);
    printf("tête : %d\n", top(&s));
    pop(&s);
    pop(&s);
    printStack(&s);
    printf("Vide ? %d \n", isEmpty(&s));

    return 0;
}
