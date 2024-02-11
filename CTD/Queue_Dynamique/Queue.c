#include "Queue.h"
#include <assert.h>


struct d_queue {
	int value;
	struct d_queue *next;
};


struct s_queue {

	Maillon *head;
	Maillon *tail;
	unsigned int size;

};


Maillon *createMaillon() {
	return malloc(sizeof(Maillon));
}


Queue *createQueue() {
	Queue *q = malloc(sizeof(Queue));
	if (q == NULL) return NULL;
	q->head = q->tail = NULL;
	q->size = 0;

	return(q);
}


Queue *push(Queue *q, int v){
	Maillon **new_emplacement = (q->size ? &(q->tail->next) : &(q->head));
	// Maillon **new_emplacement = isEmpty(q)? &(q->head) : &(q->tail->next);
	Maillon *new = createMaillon();
	new->value = v;
	new->next = NULL;
	*new_emplacement = new;
	q->tail = new;
	++(q->size);

	return(q);
}


Queue *pop(Queue *q) {
	assert(!isEmpty(q));
	Maillon *old = q->head;
	q->head = q->head->next;
	--(q->size);
	free(old);
	old = NULL;
	
	return(q);

}

//Opérateurs d'état du TAD Queue
bool isEmpty (const Queue *q) {
	return q->size == 0;
}


int getHead(Queue *q) {
	assert(!isEmpty(q));
	return q->head->value;
}


int getTail(Queue *q) {
	assert(!isEmpty(q));
	return q->tail->value;
}


int getSize(Queue *q) {
	return q->size;
}


void printQueue(Queue *q) {
	for (Maillon *m = q->head; m != q->tail->next; m = m->next) {
		printf("%d ", m->value);
	}

	printf("\n");
}


void freeMaillon(Maillon **m) {
	if (*m != NULL) free(*m);
	*m = NULL;
}


void freeQueue(Queue **q) {
	if (*q != NULL) {
		Maillon *toRemove;
		Maillon *m = (*q)->head;

		while (m != (*q)->tail->next) {
			toRemove = m;
			m = m -> next;
			freeMaillon(&toRemove);
		}

		(*q)->head = (*q)->tail = NULL;
		free(*q);
		*q = NULL;
	}
}
