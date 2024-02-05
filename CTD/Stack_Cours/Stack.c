#include "Stack.h"
#include <assert.h>

/** Create and initialize a stack.
 */
void createStack(Stack *s) {
    s->top = -1;
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

/** Return the top of a stack
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
    return s->stack[s->top];
}

/** Print the stack
 */
void printStack(Stack *s) {
    int i = 0;
    while(i <= s->top) {printf("%d ", s->stack[i++]);};
    printf("\n");
}
