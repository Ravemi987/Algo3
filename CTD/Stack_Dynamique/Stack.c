#include "Stack.h"
#include <assert.h>

/** Create and an element of a stack.
 */
Element *createElement(void) {
    return malloc(sizeof(Element));
}

/** Create and initialize a stack.
 */
Stack *createStack(void) {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) return NULL;
    s->top = NULL;
    s->size = 0;
    return s;
}

/** Test if a stack is empty
 */
bool isEmpty(const Stack *s) {
    return s->top == NULL;
}

/** Push a value on the stack
 * @pre push(s, e) defined if s->top < STACK_SIZE
 */
void push(Stack *s, int e) {
    Element *new = createElement();
    new->value = e;
    new->next = s->top;
    s->top = new;
    ++(s->size);
}

/** Pop a value from the stack
 * @pre pop(s) defined if NOT isEmpty(s)
 */
void pop(Stack *s) {
    assert(!isEmpty(s));
    Element *toRemove = s->top;
    s->top = (toRemove)->next;
    freeElement(&toRemove);
    --(s->size);
}

/** Return the INDEX of the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int top(Stack *s) {
    assert(!isEmpty(s));
    return (s->size)-1;
}

/** Return the ELEMENT on the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int get(Stack *s) {
    assert(!isEmpty(s));
    return s->top->value;
}

/** Print the stack
 */
void printStack(Stack *s) {
    for (Element *e = s->top; e != NULL; e = e->next) {
        printf("%d ", e->value);
    }

    printf("\n");
}

/** Free the stack
 */
void freeStack(Stack **s) {
    if (*s != NULL) {
        Element *toRemove;
        Element *e = (*s)->top;

        while (e != NULL) {
            toRemove = e;
            e = e-> next;
            freeElement(&toRemove);
        }

        (*s)->top = NULL;
        free(*s);
        *s = NULL;
    }
}

/** Free an element of the stack
 */
void freeElement(Element **e) {
    if (*e != NULL) {free(*e);};
    *e = NULL;
}
