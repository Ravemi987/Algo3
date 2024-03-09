/* Garde pour éviter l'inclusion multiple */
#ifndef __STACK_H__
#define __STACK_H__

/* Inclusion des dépendances */
#include <stdbool.h>
#include <stdio.h>

/* Déclarations de constantes symboliques */
#define MAX_SIZE 16

/* Déclarations des types*/
typedef struct s_stack Stack;

/* Déclaration des fonctions */
Stack *createStack();
Stack *push(Stack *s, int e);
bool isEmpty(Stack *s);
int top(Stack *s);
int get(Stack *s);
Stack *pop(Stack *s);
void printStack(Stack *s);
void freeStack(Stack **s);

#endif
