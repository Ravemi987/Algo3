#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"

#define FIRST_LEVEL 0
#define NEGATIVE_INF -100


typedef struct s_Node {
	int value; /* Clé */
	int level; /* Niveau du noeud*/
	/* Tableaux de pointeurs de taille level */
	struct s_Node **next;
	struct s_Node **previous;
} Node;


struct s_SkipList {
	int MaxLevel; /* Nombre de niveaux dans la skipList*/
	Node *sentinel;
	RNG seed; /* Graine de génération aléatoire */
	int size;
};


SkipList skiplist_create(int nbLevels) {
	SkipList d = (SkipList)malloc(sizeof(struct s_SkipList)); /* Allocation skipList*/
	if (d == NULL) return NULL;

	d->sentinel = (Node*)malloc(sizeof(Node)); /* Allocation sentinelle*/
	if (d->sentinel == NULL) return NULL;

	d->seed = rng_initialize(0);
	d->size = 0;
	d->MaxLevel = d->sentinel->level = nbLevels;
	d->sentinel->value = NEGATIVE_INF;

	/* Initialisation de la sentinelle */
	d->sentinel->next = (Node**)malloc(nbLevels*sizeof(Node*)); /* Allocation du tableau de pointeurs next */
	if (d->sentinel->next == NULL) return NULL;

	d->sentinel->previous = (Node**)malloc(nbLevels*sizeof(Node*)); /* Allocation du tableau de pointeurs previous */
	if (d->sentinel->previous == NULL) return NULL;

	/* Initialisation des tableaux de pointeurs*/
	for (int i = 0; i < nbLevels; i++) {
		d->sentinel->next[i] = d->sentinel->previous[i] = d->sentinel;
	}

	return d;
}

static void skiplist_freeNode(Node **n) {
	if (*n != NULL) {
		free((*n)->next);
		free((*n)->previous);
		free(*n);
		*n = NULL;
	}
}

void skiplist_delete(SkipList d) {
	Node *toDelete;
	Node *current = d->sentinel->next[FIRST_LEVEL];

	while (current != d->sentinel) {
		toDelete = current;
		current = current->next[FIRST_LEVEL];
		skiplist_freeNode(&toDelete); /* Linération du noeud */
	}

	skiplist_freeNode(&current); /* Libération de la sentinelle */
	free(d); /* Libération de la skiplist */
}

unsigned int skiplist_size(SkipList d) {
	return d->size;
}

int skiplist_ith(SkipList d, unsigned int i) {
	Node *node = d->sentinel->next[FIRST_LEVEL];
	while (i--) node = node->next[FIRST_LEVEL];
	return node->value;
}

void skiplist_map(SkipList d, ScanOperator f, void *user_data) {
	for (Node *node = d->sentinel->next[FIRST_LEVEL]; node != d->sentinel; node = node->next[FIRST_LEVEL])
		f(node->value, user_data);
}

Node* makeNode(int level, int value) {
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) return NULL;

	n->next = (Node**)malloc(level*sizeof(Node*));
	if (n->next == NULL) return NULL;

	n->previous = (Node**)malloc(level*sizeof(Node*));
	if (n->previous == NULL) return NULL;

	n->level = level;
	n->value = value;
	
	return n;
}

SkipList skiplist_insert(SkipList d, int value) {
	Node* update[d->MaxLevel];
	if (update == NULL) return NULL;
	int start_level = (d->MaxLevel)-1; // ou (d->sentinel->level)-1
	Node* node = d->sentinel;

	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (node->next[i] != d->sentinel && node->next[i]->value < value)
			node = node->next[i];
		update[i] = node;
	}

	/* value >= a la valeur du noeud courant */
	node = node->next[FIRST_LEVEL];

	if (node->value != value) {
		int level = (int)rng_get_value(&(d->seed), (d->MaxLevel)-1);
		/* Le niveau ne sera pas supérieur au niveau maximal de la liste */
		Node* newNode = makeNode(level+1, value);
		for (int i = 0; i <= level; i++) {
			newNode->next[i] = update[i]->next[i];
			newNode->previous[i] = update[i]->next[i]->previous[i];
			update[i]->next[i]->previous[i] = newNode;
			update[i]->next[i] = newNode;
			
		}
		(d->size)++;
	}

	return d;
}

bool skiplist_search(SkipList d, int value, unsigned int *nb_operations) {
	int start_level = (d->MaxLevel)-1;
	Node* current_node = d->sentinel;
	*nb_operations = 0;

	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (current_node->next[i] != d->sentinel && current_node->next[i]->value < value) {
			current_node = current_node->next[i];
			(*nb_operations)++;
		}
	}
	current_node = current_node->next[FIRST_LEVEL];
	if (current_node != d->sentinel) (*nb_operations)++;
	return current_node->value == value;
}

