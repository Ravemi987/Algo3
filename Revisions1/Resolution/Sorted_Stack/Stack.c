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

Node *createNode(int e) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) return NULL;
    n->value = e;
    n->next = NULL;
    return n;
}

Stack *push(Stack *s, Node *node) {
    if (s->size == 0) {
        node->next = s->top;
        s->top = node;
        ++(s->size);
        return s;
    } else {
        if (s->top->value >= node->value) {
            node->next = s->top;
            s->top = node;
        } else {
            Node *tmp_node = createNode(s->top->value);
            s = pop(s);
            push(s, node);
            tmp_node->next = s->top;
            s->top = tmp_node;
        }
        ++(s->size);
        return s;
    }
}

bool isEmpty(Stack *s) {
    return s->size == 0;
}

int getSize(Stack *s) {
    return s->size;
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