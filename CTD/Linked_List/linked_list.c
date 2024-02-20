#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "linked_list.h"


List *list() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL;
	l->head = l->tail = NULL;
	l->size = 0;

	return l;
}

List *push_back(List *l, int v) {
	Maillon **insert_at = (l->size ? &(l->tail->next) : &(l->head));
	Maillon *new = malloc(sizeof(Maillon));
	new->value = v;
	new->next = NULL; new->previous = l->tail;
	*insert_at = new;
	l->tail = new;
	++(l->size);

	return l;
}

List *push_front(List *l, int v) {
	Maillon **insert_at = (l->size ? &(l->head->previous) : &(l->tail));
	Maillon *new = malloc(sizeof(Maillon));
	new->value = v;
	new->next = l->head; new->previous = NULL;
	*insert_at = new;
	l->head = new;
	++(l->size);

	return l;
}

List *pop_back(List *l) {
	assert(!list_empty(l));
	Maillon *toRemove = l->tail;
	l->tail = toRemove->previous; // !!!
	if (l->tail) {
		l->tail->next = NULL;
	} else {
		l->head = l->tail;
	}
	free(toRemove);
	--(l->size);

	return l;
}

List *pop_front(List *l) {
	assert(!list_empty(l));
	Maillon *toRemove = l->head;
	l->head = toRemove->next; // !!!
	if (l->head) {
		l->head->previous = NULL;
	} else {
		l->tail = l->head;
	}
	free(toRemove);
	--(l->size);

	return l;
}

bool list_empty (const List *l) {
	return l->size == 0;
}

int front(const List *l) {
	assert(!list_empty(l));
	return l->head->value;
}

int back(const List *l) {
	assert(!list_empty(l));
	return l->tail->value;
}

int at(const List *l, int p) {
	assert(!list_empty(l) && p >= 0 && p <= l->size);
	if (p == 0) {
		return front(l);
	} else if (p == l->size) {
		return back(l);
	} else {
		Maillon *cell = l->head;

		while (p--) {
			cell = cell->next;
		}

		return cell->value;
	}
}

List *insert_at(List *l , int p, int v) {
	assert(p >= 0 && p <= l->size);
	if (p == 0) {
		return push_front(l, v);
	} else if (p == l->size) {
		return push_back(l, v);
	} else {
		Maillon *cell = l->head;

		while (p--) {
			cell = cell->next;
		}

		Maillon *new = malloc(sizeof(Maillon));
		new->value = v;
		new->next = cell;
		new->previous = cell->previous;
		cell->previous->next = new;
		cell->previous = new;
		++(l->size);

		return l;
	}
}

List *remove_at(List *l , int p) {
	assert(!list_empty(l) && p >= 0 && p <= l->size);
	if (p == 0) {
		return pop_front(l);
	} else if (p == l->size) {
		return pop_back(l);
	} else {
		Maillon *cell = l->head;

		while (p--) {
			cell = cell->next;
		}

		Maillon *toRemove = cell;
		cell->previous->next = cell->next;
		cell->next->previous = cell->previous;
		free(toRemove);
		--(l->size);

		return l;
	}
}

void freeList(List **l) {
	if (*l != NULL) {
		Maillon *toRemove;
		Maillon *cell = (*l)->head; // !!!

		while(cell != NULL) {
			toRemove = cell;
			cell = cell->next;
			free(toRemove);
		}

		(*l)->head = (*l)->tail = NULL; // !!!
		free(*l);
		(*l) = NULL;
	}
}


int main() {

	List* l = list();

	int i;

	for(i=0;i<10;i++) l = push_back(l,i);
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

	printf("###########################\n");
    printf("###########################\n");
    printf("size  %d \n",l->size);
    printf("###########################\n");

	printf("cas 1 : ajoute en fin de liste\n");
	l =  insert_at(l, l->size,100);	
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );


	printf("###########################\n");

	printf("cas 2 :ajout liste vide (creation nouvelle liste) \n");
	List* l2 = list();
	l2 =  insert_at(l2, 0,100);	
	printf("at %d\n",at(l2,0) );

	printf("###########################\n");


	printf("cas 2.2 :ajout debut de liste \n");
	l =  insert_at(l, 0,200);	
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );


	printf("###########################\n");


	printf("cas 3 :ajout au milieu \n");
	l =  insert_at(l, 2,300);	
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

	printf("###########################\n");



	printf("###########################\n");	
	printf("###########################\n");
    printf("suppression \n");

    printf("###########################\n");
    printf("Suppression 1er element \n");
	l =  remove_at(l, 0);
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );
    printf("###########################\n");
   printf("size  %d \n",l->size);
    printf("###########################\n");
    printf("Suppression dernier element \n");
    l =  remove_at(l, l->size);
    for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );
    printf("###########################\n");
    printf("Suppression element indice 1 \n");
    l =  remove_at(l, 1);
    for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

	freeList(&l2);
	freeList(&l);
	freeList(&l);

	if (l == NULL) {
		printf("Memoire liberee\n");
	} else {
		printf("Erreur liberation memoire\n");
	}


return(0);

}



// val = exp1 ? exp 2 : exp 3 
// if exp1 = 0 val = exp2 else val = exp3
// Exemple 
// (a%2==0)? 1:0; renvoi 1 si a est divisible par 2 sion 0
// (a>b) ? ((a>c)?a:c):((b>c)?b:c) renvoi la plus grande valeur entre a,b et c
// (x>y) ? x--:y--
