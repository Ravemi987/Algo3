#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"


typedef struct s_Node {
	int value; /* Clé */
	int level; /* Niveau du noeud*/
	/* Tableau de pointeurs de taille level */
	struct s_Node **next;
	struct s_Node **previous;
} Node;


struct s_SkipList {
	int nblevels; /* Nombre de niveaux dans la skipList*/
	Node *sentinel;
	RNG seed; /* Graine de génération aléatoire */
	int size;
};


SkipList skiplist_create(int nblevels) {
	SkipList d = malloc(sizeof(struct s_SkipList)); /* Allocation skipList*/
	if (d == NULL) return NULL; /* Erreur d'allocation */
	d->sentinel = malloc(sizeof(Node)); /* Allocation sentinelle*/
	if (d->sentinel == NULL) return NULL; /* Erreur d'allocation */
	d->seed = rng_initialize(0);
	d->size = 0;
	d->nblevels = d->sentinel->level = nblevels;
	/* Initialisation de la sentinelle */
	d->sentinel->next = malloc(sizeof(Node*)); /* Allocation du tableau de pointeurs next */
	if (d->sentinel->next == NULL) return NULL;
	d->sentinel->previous = malloc(sizeof(Node*)); /* Allocation du tableau de pointeurs previous */
	if (d->sentinel->previous == NULL) return NULL;
	/* Initialisation des tableaux de pointeurs*/
	for (int i = 0; i < nblevels; i++) {
		d->sentinel->next[i] = d->sentinel->previous[i] = d->sentinel;
	}
}

static void skiplist_freeNode(Node **n) {
	if (*n != NULL) {
		for (int i = 0; i < (*n)->level; i++) {
			free((*n)->next[i]);
			free((*n)->previous[i]);
		}
		free(*n);
		*n = NULL;
	}
}

void skiplist_delete(SkipList d) {
	Node *toDelete;
	Node *current = d->sentinel->next;

	while (current != d->sentinel) {
		toDelete = current;
		current = current->next;
		/* On libère les tableaux de pointeurs du noeud */
		skiplist_freeNode(&toDelete);
	}

	skiplist_freeNode(&current); /* Libération de la sentinelle */
	free(d);
}

unsigned int skipList_size(SkipList d) {
	return d->size;
}

SkipList skiplist_insert(SkipList d, int value) {
	(void)value;
	return d;
}
