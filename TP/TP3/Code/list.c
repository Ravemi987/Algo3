/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement* whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement* sentinel;
	int size;
};

/* define the SubList structure. Its implementation is a list without sentinel */
typedef struct s_SubList {
	LinkedElement* head;
	LinkedElement* tail;
} *SubList;


/*-----------------------------------------------------------------*/

List* list_create() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL;

	l->sentinel = malloc(sizeof(LinkedElement));
	if (l->sentinel == NULL) return NULL;

	l->sentinel->next = l->sentinel->previous = l->sentinel;
	l->size = 0;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {
	LinkedElement *new = malloc(sizeof(LinkedElement));
	new->previous = l->sentinel->previous;
	new->next = l->sentinel;
	new->value = v;
	new->previous->next = new;
	l->sentinel->previous = new;
	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList *l) {
	if (*l != NULL) {
		LinkedElement *toRemove;
		LinkedElement *current = (*l)->sentinel->next;

		while (current != (*l)->sentinel) {
			toRemove = current;
			current = current->next;
			free(toRemove);
		}

		free((*l)->sentinel);
		free(*l);
		*l = NULL;
	}  
}

/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
	LinkedElement *new = malloc(sizeof(LinkedElement));
	new->previous = l->sentinel;
	new->next = l->sentinel->next;
	new->value = v;
	new->next->previous = new;
	l->sentinel->next = new;
	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	assert(!list_is_empty(l));
	return l->sentinel->next->value;
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	assert(!list_is_empty(l));
	return l->sentinel->previous->value;
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
	assert(!list_is_empty(l));
	LinkedElement *toRemove = l->sentinel->next;
	toRemove->next->previous = l->sentinel;
	l->sentinel->next = toRemove->next;
	free(toRemove);
	--(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	assert(!list_is_empty(l));
	LinkedElement *toRemove = l->sentinel->previous;
	toRemove->previous->next = l->sentinel;
	l->sentinel->previous = toRemove->previous;
	free(toRemove);
	--(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
	assert(0 <= p && p <= l->size);
	if (p == 0) {
		return list_push_front(l, v);
	} else if (p == l->size) {
		return list_push_back(l, v);
	} else {
		LinkedElement *current = l->sentinel->next;
		LinkedElement *new = malloc(sizeof(LinkedElement));
		while (p--) current = current->next;
		new->previous = current->previous;
		new->next = current;
		new->value = v;
		current->previous->next = new;
		current->previous = new;
		++(l->size);
		return l;
	}
}

/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
	assert(!list_is_empty(l) && 0 <= p && p <= l->size);
	if (p == 0) {
		return list_pop_front(l);
	} else if (p == l->size) {
		return list_pop_back(l);
	} else {
		LinkedElement *toRemove;
		LinkedElement *current = l->sentinel->next;
		while (p--) current = current->next;
		toRemove = current;
		current->previous->next = current->next;
		current->next->previous = current->previous;
		--(l->size);
		free(toRemove);
		return l;
	}
}

/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	assert(!list_is_empty(l) && 0 <= p && p <= l->size);
	LinkedElement *current = l->sentinel->next;
	while (p--) current = current->next;
	return current->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
	return l->size == 0;
}

/*-----------------------------------------------------------------*/

int list_size(const List* l) {
	return l->size;
}

/*-----------------------------------------------------------------*/

List* list_map(List* l, SimpleFunctor f) {
	for (LinkedElement *e = l->sentinel->next; e != l->sentinel; e = e->next) {
		e->value = f(e->value);
	}
	return l;
}


List* list_reduce(List* l, ReduceFunctor f, void *userData) {
	for (LinkedElement *e = l->sentinel->next; e != l->sentinel; e = e->next) {
		f(e->value, userData);
	}
	return l;
}

/*-----------------------------------------------------------------*/

SubList list_split(SubList l) {
	/* Le pointeur slow avance deux fois plus lentement que le pointeur fast*/
	LinkedElement *slowPointer = l->head;
	LinkedElement *fastPointer = l->head->next;

	/* Lorsque fast a atteint la fin de la list, slow pointe le milieu */
	while (fastPointer != NULL) {
		fastPointer = fastPointer->next;
		if(fastPointer != NULL) {
			slowPointer = slowPointer->next;
			fastPointer = fastPointer->next;
		}
	}

	SubList newList = l;
	newList->head = l->head;
	/* La queue de la liste pointe vers la sous-liste droite*/
	newList->tail = slowPointer->next;
	/* Les sous-listes de gauche et de droite sont de nouvelles listes
	 * donc slow pointe la queue de la sous-liste gauche */
	slowPointer->next = NULL;
	return newList;
}

/*-----------------------------------------------------------------*/

SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
	if (leftlist == NULL) {
		return rightlist;
	} else if (rightlist == NULL) {
		return leftlist;
	} else {
		SubList tmp;
		if (f(leftlist->head->value, rightlist->head->value)) {
			/* tmp est la sous-liste dont la tête de la sous-liste gauche est décalée*/
			tmp = leftlist;
			tmp->head = leftlist->head->next;
			leftlist = list_merge(tmp, rightlist, f);
			/* On raccorde la nouvelle tête avec le reste de la liste qui sera triée */
			leftlist->head->next->previous = leftlist->head;
			leftlist->head->previous = NULL;
			return leftlist;
		} else {
			/* On fait la même chose que pour la sous-liste gauche */
			tmp = rightlist;
			tmp->head = rightlist->head->next;
			rightlist = list_merge(leftlist, tmp, f);
			rightlist->head->next->previous = leftlist->head;
			rightlist->head->previous = NULL;
			return rightlist;
		}
	}
}

/*-----------------------------------------------------------------*/

SubList list_mergesort(SubList l, OrderFunctor f) {
	if (l->head == NULL || l->head->next == NULL) {
		return l;
	} else {
		SubList splitList = list_split(l);
		SubList leftlist = l;
		leftlist->tail = splitList->tail->previous;
		SubList rightlist = l;
		rightlist->head = l->tail;
		return list_merge(list_mergesort(leftlist, f), list_mergesort(rightlist, f), f);
	}
}

/*-----------------------------------------------------------------*/

List* list_sort(List* l, OrderFunctor f) {
	SubList sl = list_mergesort((SubList)l, f);
	l->sentinel->next = sl->head;
	l->sentinel->previous = sl->tail;
	l->sentinel->next->previous = l->sentinel;
	l->sentinel->previous->next = l->sentinel;
	return l;
}

