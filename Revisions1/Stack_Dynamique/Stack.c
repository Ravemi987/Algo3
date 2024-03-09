#include "Stack.h"
#include <stdlib.h>
#include <assert.h>


struct s_node {
    int value;
    struct s_node *next;
};

struct s_stack {
    struct s_node *top;
    int size;
};


Stack *createStack() {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) return NULL;
    s->top = NULL;
    s->size = 0;
    return s;
}

Stack *push(Stack *s, int e) {
    Node *new = malloc(sizeof(Node));
    new->value = e;
    new->next = s->top;
    s->top = new;
    ++(s->size);
    return s;
}

bool isEmpty(Stack *s) {
    return s->size == 0;
}

int top(Stack *s) {
    assert(!isEmpty(s));
    return (s->size) - 1;
}

int get(Stack *s) {
    assert(!isEmpty(s));
    return s->top->value;
}

Stack *pop(Stack *s) {
    assert(!isEmpty(s));
    Node *toDelete = s->top;
    s->top = s->top->next;
    free(toDelete);
    toDelete = NULL;
    --(s->size);
    return s;
}

void printStack(Stack *s) {
    for (Node *e = s->top; e != NULL; e = e->next) {
        printf("%d ", e->value);
    }
    printf("\n");
}

void freeStack(Stack **s) {
    if (*s != NULL) {
        Node *toDelete;
        Node *current = (*s)->top;

        while (current != NULL) {
            toDelete = current;
            current = current ->next;
            free(toDelete);
            toDelete = NULL;
        }

        free(*s);
        *s = NULL;
    }
}