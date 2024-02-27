#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "linked_list.h"


List *list() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL;
	l->sentinel = malloc(sizeof(Maillon));
	if (l->sentinel == NULL) return NULL;
	l->sentinel->previous = l->sentinel->next = l->sentinel; // !!!
	l->size = 0;

	return l;
}

List *push_back(List *l, int v) {
	Maillon *new = malloc(sizeof(Maillon));
	new->value = v;
	new->next = l->sentinel; new->previous = l->sentinel->previous;
	l->sentinel->previous->next = new; // !!! new->previous->next = new
	l->sentinel->previous = new;
	++(l->size);

	return l;
}

List *push_front(List *l, int v) {
	Maillon *new = malloc(sizeof(Maillon));
	new->value = v;
	new->next = l->sentinel->next; new->previous = l->sentinel;
	l->sentinel->next->previous = new; // !!! new->next->previous = new
	l->sentinel->next = new;
	++(l->size);

	return l;
}

List *pop_back(List *l) {
	assert(!list_empty(l));
	Maillon *toRemove = l->sentinel->previous;
	toRemove->previous->next = l->sentinel;
	l->sentinel->previous = toRemove->previous;
	free(toRemove);
	--(l->size);

	return l;	
}

List *pop_front(List *l) {
	assert(!list_empty(l));
	Maillon *toRemove = l->sentinel->next;
	toRemove->next->previous = l->sentinel; // l->sentinel->next->previous = l->sentinel si on inverse les deux lignes
	l->sentinel->next = toRemove->next;
	free(toRemove);
	--(l->size);

	return l;
}

bool list_empty (const List *l) {
	return l->size == 0;
}

int front(const List *l) {
	assert(!list_empty(l));
	return l->sentinel->next->value;
}

int back(const List *l) {
	assert(!list_empty(l));
	return l->sentinel->previous->value;
}

int at(const List *l, int p) {
	assert(!list_empty(l) && p >= 0 && p <= l->size);
	if (p == 0) {
		return front(l);
	} else if (p == l->size) {
		return back(l);
	} else {
		Maillon *cell = l->sentinel->next;

		while (p--) {
			cell = cell -> next;
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
		Maillon *cell = l->sentinel->next;

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
		Maillon *cell = l->sentinel->next;

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
		Maillon *cell = (*l)->sentinel->next; // !!!

		while (cell != (*l)->sentinel) { // !!!
			toRemove = cell;
			cell = cell->next;
			free(toRemove);
		}

		free((*l)->sentinel);
		(*l)->sentinel = NULL;
		free(*l);
		(*l) = NULL;
	}
}



List* map(List* l, SimpleFunctor f) {
	for (Maillon* e = l->sentinel->next; e != l->sentinel; e = e->next) {
		e->value = f(e->value);
	}

	return l;
}

List* reduce(List* l, ReduceFunctor f, void* userEnv) {
	for (Maillon* e = l->sentinel->next; e != l->sentinel; e = e->next) {
		f(e->value, userEnv);
	}

	return l;
}



int print_e(int e) {
	printf("%d\n", e);
	return e;
}

int mult_e(int e) {
	e *= e;
	return e;
}

void accumulate(int i, void* env) {
	int *r = (int*)env;
	*r += i;
}

void multiplicate(int i, void* env) {
	int *r = (int*)env;
	*r *= i;
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
    printf("Map / Reduce \n");

	l = map(l, print_e);
	printf("\n");
	int s = 0, p = 0;
	l = reduce(l, accumulate, &s);
	printf("sum : %d\n", s);
	l = reduce(l, multiplicate, &p);
	printf("mult : %d\n", p);

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
