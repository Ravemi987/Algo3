/* Garde pour éviter l'inclusion multiple */
#ifndef __STACK_H__
#define __STACK_H__

/* Inclusion des dépendances */
#include <stdbool.h>
#include <stdio.h>

/* Déclarations des types*/
typedef struct s_node Node;
typedef struct s_stack Stack;

/* Déclaration des fonctions */
Stack *createStack();
Node *createNode(int e);
Stack *push(Stack *s, Node *n);
bool isEmpty(Stack *s);
int getSize(Stack *s);
int top(Stack *s);
int get(Stack *s);
Stack *pop(Stack *s);
void printStack(Stack *s);
void freeStack(Stack **s);

#endif
