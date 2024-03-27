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
} SubList;


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
	LinkedElement *slowPointer = l.head;
	LinkedElement *fastPointer = l.head->next;

	/* Lorsque fast a atteint la fin de la list, slow pointe le milieu */
	while (fastPointer != NULL) {
		fastPointer = fastPointer->next;
		if(fastPointer != NULL) {
			slowPointer = slowPointer->next;
			fastPointer = fastPointer->next;
		}
	}

	SubList temp = l;
	temp.head = slowPointer;
	temp.tail = slowPointer->next;
	slowPointer->next = NULL;
	return temp;
}

/*-----------------------------------------------------------------*/

SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
	SubList result;
	if (f(leftlist.head->value, rightlist.head->value)) {
		result.head = leftlist.head;
		leftlist.head = leftlist.head->next;
	} else {
		result.head = rightlist.head;
		rightlist.head = rightlist.head->next;
	}

	LinkedElement *new = result.head;

	while (leftlist.head->next && rightlist.head->next) {
		if (f(leftlist.head->value, rightlist.head->value)) {
			new->next = leftlist.head;
			new = new->next;
			leftlist.head = leftlist.head->next;
		} else {
			new->next = rightlist.head;
			new = new->next;
			rightlist.head = rightlist.head->next;
		}
	}
	if (rightlist.head->next) {
		while (rightlist.head->next) {
			new->next = rightlist.head;
			new = new->next;
			rightlist.head = rightlist.head->next;
		}
		result.tail = rightlist.head;
	} else if (leftlist.head->next) {
		while (leftlist.head->next) {
			new->next = leftlist.head;
			new = new->next;
			leftlist.head = leftlist.head->next;
		}
		result.tail = leftlist.head;
	} else {
		result.tail = new;
	}
	return result;
}

/*-----------------------------------------------------------------*/

SubList list_mergesort(SubList l, OrderFunctor f) {
	if (l.head == NULL || l.head->next == NULL) {
		return l;
	} else {
		SubList leftlist, rightlist;
		leftlist = rightlist = l;
		SubList splitList = list_split(l);
		leftlist.tail = splitList.head;
		rightlist.head = splitList.tail;
		return list_merge(list_mergesort(leftlist, f), list_mergesort(rightlist, f), f);
	}
}

/*-----------------------------------------------------------------*/

List* list_sort(List* l, OrderFunctor f) {
	SubList *sl = malloc(sizeof(SubList));
	if (sl == NULL) return NULL;
	sl->head = l->sentinel->next;
	sl->tail = l->sentinel->previous;
	SubList result = list_mergesort(*sl, f);
	l->sentinel->next = result.head;
	l->sentinel->previous = result.tail;
	l->sentinel->next->previous = l->sentinel;
	l->sentinel->previous->next = l->sentinel;
	return l;
}

