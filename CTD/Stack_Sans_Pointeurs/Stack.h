#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 16

typedef struct {
    int stack[STACK_SIZE];
    int top;
} Stack;

/** Create and initialize a stack.
 */
Stack createStack(void);

/** Test if a stack is empty
 */
bool isEmpty(const Stack s);

/** Push a value on the stack
 * @pre push(s, e) defined if s->top < STACK_SIZE
 */
Stack push(Stack s, int e);

/** Pop a value from the stack
 * @pre pop(s) defined if NOT isEmpty(s)
 */
Stack pop(Stack s);

/** Return the INDEX of the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int top(Stack s);

/** Return the ELEMENT on the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int get(Stack s);

/** Print the stack
 */
void printStack(Stack s);

#endif
