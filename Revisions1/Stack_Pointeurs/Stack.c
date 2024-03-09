#include "Stack.h"
#include <stdlib.h>
#include <assert.h>


struct s_stack {
    int top;
    int stack[MAX_SIZE];
};


Stack *createStack() {
    Stack *s = malloc(sizeof(Stack));
    s->top = -1;
    return s;
}

Stack *push(Stack *s, int e) {
    assert(s->top < MAX_SIZE);
    s->stack[++(s->top)] = e;
    return s;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

int top(Stack *s) {
    assert(!isEmpty(s));
    return s->top;
}

int get(Stack *s) {
    assert(!isEmpty(s));
    return s->stack[s->top];
}

Stack *pop(Stack *s) {
    assert(!isEmpty(s));
    --(s->top);
    return s;
}

void printStack(Stack *s) {
    for (int i = 0; i <= s->top; i++) {
        printf("%d ", s->stack[i]);
    }
    printf("\n");
}

void freeStack(Stack **s) {
    if (*s != NULL) {
        free(*s);
        *s = NULL;
    }
}