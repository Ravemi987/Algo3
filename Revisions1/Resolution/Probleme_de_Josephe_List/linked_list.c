#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct s_node {
	int value;
	struct s_node *next;
	struct s_node *previous;
};

struct s_list {
	struct s_node *head;
	struct s_node *tail;
	int size;
};


List *list() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL;
	l->head = l->tail = NULL;
	l->size = 0;
	return l;
}

List *push_front(List *l, int v) {
	Node **insert_pos = (l->size ? &(l->head->previous) : &(l->tail));
	Node *new = malloc(sizeof(Node));
	new->value = v;
	new->next = l->head;
	new->previous = NULL;
	*insert_pos = new;
	l->head = new;
	++(l->size);
	return l;
}

List *push_back(List *l, int v) {
	Node **insert_pos = (l->size ? &(l->tail->next) : &(l->head));
	Node *new = malloc(sizeof(Node));
	new->value = v;
	new->next = NULL;
	new->previous = l->tail;
	*insert_pos = new;
	l->tail = new;
	++(l->size);
	return l;
}

List *insert_at(List *l, int i, int v) {
	assert(0 <= i && i <= l->size);
	if (i == 0) {
		return push_front(l, v);
	} else if (i == l->size) {
		return push_back(l, v);
	} else {
		Node *current = l->head;
		while (i--) current = current->next;
		Node *new = malloc(sizeof(Node));
		new->value = v;
		new->next = current;
		new->previous = current->previous;
		current->previous->next = new;
		current->previous = new;
		++(l->size);
		return l;
	}
}

bool is_empty(List *l) {
	return l->size == 0;
}

List *pop_front(List *l) {
	assert(!is_empty(l));
	Node *toDelete = l->head;
	l->head = l->head->next;
	if (l->head) {
		l->head->previous = NULL;
	} else {
		l->tail = l->head;
	}
	free(toDelete);
	toDelete = NULL;
	--(l->size);
	return l;
}

List *pop_back(List *l) {
	assert(!is_empty(l));
	Node *toDelete = l->tail;
	l->tail = l->tail->previous;
	if (l->tail) {
		l->tail->next = NULL;
	} else {
		l->head = l->tail;
	}
	free(toDelete);
	toDelete = NULL;
	--(l->size);
	return l;	
}

List *remove_at(List *l, int i) {
	assert(!is_empty(l) && 0 <= i && i <= l->size);
	if (i == 0) {
		return pop_front(l);
	} else if (i == l->size) {
		return pop_back(l);
	} else {
		Node *current = l->head;
		while(i--) current = current->next;
		Node *toDelete = current;
		current->next->previous = current->previous;
		current->previous->next = current->next;
		free(toDelete);
		toDelete = NULL;
		--(l->size);
		return l;
	}
}

int front(List *l) {
	assert(!is_empty(l));
	return l->head->value;
}

int back(List *l) {
	assert(!is_empty(l));
	return l->tail->value;
}

int at(List *l, int i) {
	assert(!is_empty(l) && 0 <= i && i <= l->size);
	Node *current = l->head;
	while (i--) current = current->next;
	return current->value;
}

void freeList(List **l) {
	if (*l != NULL) {
		Node *toDelete;
		Node *current = (*l)->head;

		while (current != NULL) {
			toDelete = current;
			current = current->next;
			free(toDelete);
			toDelete = NULL;
		}

		(*l)->head = (*l)->tail = NULL;
		free(*l);
		*l = NULL;
	}
}

List *represent(int n) {
	List *l = list();
	for (int i = n; i >= 1; i--)
		l = push_front(l, i);
	return l;
}

int solve(List *l, int k) {
	int i = 0;
	while (l->size > 1) {
		for(int i=0;i<l->size;i++) printf("%d ",at(l,i));
		printf("\n");
		int todelete = (i+k) % (l->size);
		l = remove_at(l, todelete);
		i = (todelete + l->size - 1) % (l->size);
	}
	return front(l);
}


int main() {

	List *l = represent(8);
	printf("\n");
	printf("Solution : %d\n", solve(l, 3));

	freeList(&l);

	if (l == NULL) {
		printf("Memoire liberee\n");
	} else {
		printf("Erreur liberation memoire\n");
	}


return(0);

}