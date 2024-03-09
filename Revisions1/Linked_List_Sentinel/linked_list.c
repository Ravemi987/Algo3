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
	struct s_node *sentinel;
	int size;
};

List *list() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL;
	l->sentinel = malloc(sizeof(Node));
	if(l->sentinel == NULL) return NULL;
	l->sentinel->next = l->sentinel->previous = l->sentinel;
	l->size = 0;
	return l; 
}

List *push_front(List *l, int v) {
	Node *new = malloc(sizeof(Node));
	new->value = v;
	new->previous = l->sentinel;
	new->next = l->sentinel->next;
	new->next->previous = new;
	l->sentinel->next = new;
	++(l->size);
	return l;
}

List *push_back(List *l, int v) {
	Node *new = malloc(sizeof(Node));
	new->value = v;
	new->previous = l->sentinel->previous;
	new->next = l->sentinel;
	new->previous->next = new;
	l->sentinel->previous = new;
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
		Node *current = l->sentinel->next;
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
	Node *toDelete = l->sentinel->next;
	toDelete->next->previous = l->sentinel;
	l->sentinel->next = toDelete->next;
	free(toDelete);
	toDelete = NULL;
	--(l->size);
	return l;
}

List *pop_back(List *l) {
	assert(!is_empty(l));
	Node *toDelete = l->sentinel->previous;
	toDelete->previous->next = l->sentinel,
	l->sentinel->previous = toDelete->previous;
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
		Node *current = l->sentinel->next;
		while (i--) current = current->next;
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
	return l->sentinel->next->value;
}

int back(List *l) {
	assert(!is_empty(l));
	return l->sentinel->previous->value;
}

int at(List *l, int i) {
	assert(!is_empty(l) && 0 <= i && i <= l->size);
	Node *current = l->sentinel->next;
	while (i--) current = current->next;
	return current->value;
}

void freeList(List **l) {
	if (*l != NULL) {
		Node *toDelete;
		Node *current = (*l)->sentinel->next;
		
		while (current != (*l)->sentinel) {
			toDelete = current;
			current = current->next;
			free(toDelete);
			toDelete = NULL;
		}

		free((*l)->sentinel);
		(*l)->sentinel = NULL;
		free(*l);
		*l = NULL;
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
	l = push_front(l, 200);
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );


	printf("###########################\n");


	printf("cas 3 :ajout au milieu \n");
	l =  insert_at(l, 2,300);	
	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

	printf("###########################\n");

	printf("affichage \n");
	printf("%d\n", front(l));
	printf("%d\n", back(l));

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
