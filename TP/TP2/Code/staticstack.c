/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD Stack étudié en cours.
 
 */
/*-----------------------------------------------------------------*/
#include "stack.h"
#include <assert.h>
#include <stdlib.h>
#define STACK_SIZE 32

/* Full definition of the s_stack structure */
struct s_stack {
	int top;
	int capacity;
	void** stack; // array of void *
};

Stack* createStack(int max_size) {
	Stack* s;
	size_t capacity = (max_size > 0 ? max_size : STACK_SIZE);
	s = malloc(sizeof(struct s_stack) + sizeof(void *) * capacity);
	s->stack = (void**)(s+1);
	s->capacity = capacity;
	s->top=-1;
	return (s);
}

void deleteStack(ptrStack *s) {
	free (*s);
	*s = NULL;
}

Stack* stackPush(Stack* s, void* e) {
	assert(s->top != STACK_SIZE-1);
	s->stack[++(s->top)] = e;
	return(s);
}

bool stackEmpty(const Stack* s) {
	return (s->top == -1);
}

Stack* stackPop(Stack* s) {
	assert(!stackEmpty(s));
	--(s->top);
	return(s);
}

void* stackTop(const Stack* s) {
	assert(!stackEmpty(s));
	return (s->stack[s->top]);
}

bool stackOverflow(const Stack* s){
	return s->top + 1 == s->capacity;
}

void stackDump(FILE* f, const Stack* s, void(*dumpfunction)(FILE *f, const void* e)) {
	fprintf(f, "(%d) --  ", s->top+1);
	for (int i=s->top; i>=0; --i)
		dumpfunction(f, s->stack[i]);
}

