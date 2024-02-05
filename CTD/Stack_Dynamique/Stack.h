#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct s_element {
    int value;
    struct s_element *next;
} Element;

typedef struct s_stack {
    int size;
    Element *top;
} Stack;


/** Create and an element of a stack.
 */
Element *createElement(void);

/** Create and initialize a stack.
 */
Stack *createStack(void);

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

/** Return the INDEX of the top of a stack
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

/** Free the stack
 */
void freeStack(Stack **s);

/** Free an element of the stack
 */
void freeElement(Element **e);

#endif
