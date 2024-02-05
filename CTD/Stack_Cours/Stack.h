#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stdio.h>

#define STACK_SIZE 16

typedef struct {
    int stack[STACK_SIZE];
    int top;
} Stack;

/** Create and initialize a stack.
 */
void createStack(Stack *s);

/** Test if a stack is empty
 */
bool isEmpty(const Stack *s);

/** Push a value on the stack
 * @pre push(s, e) defined if s->top < STACK_SIZE
 */
void push(Stack *s, int e);

/** Pop a value from the stack
 * @pre pop(s) defined if NOT isEmpty(s)
 */
void pop(Stack *s);

/** Return the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int top(Stack *s);

/** Return the ELEMENT on the top of a stack
 * @pre top(s) defined if NOT isEmpty(s)
 */
int get(Stack *s);

/** Print the stack
 */
void printStack(Stack *s);

#endif
