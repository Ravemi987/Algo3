#include "Stack.h"
#include <assert.h>

/** Create and initialize a stack.
 */
Stack *createStack(void) {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) return NULL;
    s->top = -1;
    return s;
}

/** Test if a stack is empty
 */
bool isEmpty(const Stack *s) {
    return s->top == -1;
}

/** Push a value on the stack
 * @pre push(s, e) defined if s->top < STACK_SIZE
 */
void push(Stack *s, int e) {
    assert(s->top < STACK_SIZE);
    s->stack[++(s->top)] = e;
}

/** Pop a value from the stack
 * @pre pop(s) defined if NOT isEmpty(s)
 */
void pop(Stack *s) {
    assert(!isEmpty(s));
    --(s->top);
}

/** Return the INDEX of the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int top(Stack *s) {
    assert(!isEmpty(s));
    return s->top;
}

/** Return the ELEMENT on the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int get(Stack *s) {
    assert(!isEmpty(s));
    return s->stack[top(s)];
}

/** Print the stack
 */
void printStack(Stack *s) {
    int i = 0;
    while(i <= s->top) {printf("%d ", s->stack[i++]);};
    printf("\n");
}

/** Free the stack
 */
void freeStack(Stack **s) {
    if (*s != NULL) {free(*s);};
    *s = NULL;
}
