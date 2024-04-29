#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "limits.h"

#include "skiplist.h"
#include "rng.h"

#define FIRST_LEVEL 0


typedef struct s_Node {
	int value; /* Clé ou valeur */
	int level; /* Niveau du noeud*/
	struct s_Node **next; /* Tableau pour les noeuds suivants */
	struct s_Node **previous; /* Tableau pour les noeuds précédants */
} Node;


struct s_SkipList {
	int maxLevel; /* Nombre de niveaux dans la skipList*/
	Node *sentinel;
	RNG seed; /* Graine de génération aléatoire */
	int size;
};


struct s_SkipListIterator {
	SkipList collection; /* Une skiplist*/
	Node *current; /* Le noeud pointé par l'itérateur*/
	Node *begin; /* Ici la sentinelle */
	Node *(*next)(Node *); /* Fonction permettant de parcourir la collection*/
};


/*  Alloue et initialise une SkipList */
SkipList skiplist_create(int nbLevels) {
	SkipList d = (SkipList)malloc(sizeof(struct s_SkipList)); /* Allocation skipList */
	if (d == NULL) return NULL;

	d->sentinel = (Node*)malloc(sizeof(Node)); /* Allocation sentinelle */
	if (d->sentinel == NULL) return NULL;

	d->seed = rng_initialize(0); /* Initialisation de la graine de génération à 0 */
	d->size = 0;
	d->maxLevel = d->sentinel->level = nbLevels; /* Le niveau de la sentinelle est celui de la liste */
	d->sentinel->value = INT_MAX; /* La sentinelle a pour valeur 2147483647 (voir rapport) */

	/* Initialisation de la sentinelle */
	d->sentinel->next = (Node**)malloc(nbLevels*sizeof(Node*)); /* Allocation du tableau de pointeurs next */
	if (d->sentinel->next == NULL) return NULL;

	d->sentinel->previous = (Node**)malloc(nbLevels*sizeof(Node*)); /* Allocation du tableau de pointeurs previous */
	if (d->sentinel->previous == NULL) return NULL;

	/* Initialisation des tableaux de pointeurs*/
	for (int i = 0; i < nbLevels; ++i) {
		d->sentinel->next[i] = d->sentinel->previous[i] = d->sentinel;
	}

	return d;
}


/* Libère les tableaux pointeurs d'un noeud ainsi que ce noeud en le positionnant à NULL */
static void skiplist_free_node(Node **n) {
	if (*n != NULL) {
		free((*n)->next);
		free((*n)->previous);
		free(*n);
		*n = NULL;
	}
}


/* Libère la mémoire utilisée par une SkipList */
void skiplist_delete(SkipList d) {
	Node *toDelete;
	Node *current = d->sentinel->next[FIRST_LEVEL];

	while (current != d->sentinel) {
		toDelete = current;
		current = current->next[FIRST_LEVEL];
		skiplist_free_node(&toDelete); /* Libération du noeud */
	}

	skiplist_free_node(&current); /* Libération de la sentinelle */
	free(d); /* Libération de la skiplist */
}


/* Renvoie le nombre de noeuds dans une SkipList */
unsigned int skiplist_size(SkipList d) {
	return d->size;
}


/* Retourne la valeur dans la SkipList d correspondant à l'index i */
int skiplist_ith(SkipList d, unsigned int i) {
	Node *node = d->sentinel->next[FIRST_LEVEL];
	while (i--) node = node->next[FIRST_LEVEL];
	return node->value;
}


void skiplist_map(SkipList d, ScanOperator f, void *user_data) {
	for (Node *node = d->sentinel->next[FIRST_LEVEL]; node != d->sentinel; node = node->next[FIRST_LEVEL])
		f(node->value, user_data);
}


Node *makeNode(int level, int value) {
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
	Node *update[d->maxLevel];
	int start_level = (d->maxLevel)-1; // ou (d->sentinel->level)-1
	Node *node = d->sentinel;

	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (node->next[i]->value < value)
			node = node->next[i];
		update[i] = node;
	}

	/* value >= a la valeur du noeud courant */
	node = node->next[FIRST_LEVEL];

	if (node->value != value) {
		int level = (int)rng_get_value(&(d->seed), (d->maxLevel)-1);
		/* Le niveau ne sera pas supérieur au niveau maximal de la liste */
		Node *newNode = makeNode(level+1, value);
		for (int i = 0; i <= level; ++i) {
			newNode->next[i] = update[i]->next[i];
			newNode->previous[i] = update[i]->next[i]->previous[i];
			update[i]->next[i]->previous[i] = newNode;
			update[i]->next[i] = newNode;
		}
		++(d->size);
	}

	return d;
}


SkipList skiplist_remove(SkipList d, int value) {
	int start_level = (d->maxLevel)-1;
	Node *node = d->sentinel;

	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (node->next[i]->value < value)
			node = node->next[i];
	}

	node = node->next[FIRST_LEVEL];

	if (node->value == value) {
		for (int i = 0; i < node->level; ++i) {
			node->next[i]->previous[i] = node->previous[i];
			node->previous[i]->next[i] = node->next[i];
		}
		skiplist_free_node(&node);
		--(d->size);

		/* On a pu supprimer des niveaux */
		while (d->maxLevel > 1 && d->sentinel->next[(d->maxLevel)-1] == d->sentinel)
			--(d->maxLevel);
	}

	return d;
}


bool skiplist_search(SkipList d, int value, unsigned int *nb_operations) {
	int start_level = (d->maxLevel)-1;
	Node *current_node = d->sentinel;
	*nb_operations = 1;

	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (current_node->next[i]->value < value) {
			current_node = current_node->next[i];
			++(*nb_operations);
		}
		if (current_node->next[i]->value == value)
			return true;
	}
	return false;
}


static Node *goto_next(Node *n) {
	return n->next[FIRST_LEVEL];
}


static Node *goto_previous(Node *n) {
	return n->previous[FIRST_LEVEL];
}


SkipListIterator skiplist_iterator_create(SkipList d, unsigned char w) {
	SkipListIterator it = malloc(sizeof(struct s_SkipListIterator));
	if (it == NULL) return NULL;

	it->collection = d;

	if (w == FORWARD_ITERATOR) {
		it->begin = d->sentinel->next[FIRST_LEVEL];
		it->next = goto_next;
	} else {
		it->begin = d->sentinel->previous[FIRST_LEVEL];
		it->next = goto_previous;
	}

	it->current = it->begin;

	return it;
}


void skiplist_iterator_delete(SkipListIterator it) {
	if (it) free(it);
}


SkipListIterator skiplist_iterator_begin(SkipListIterator it) {
	it->current = it->begin;
	return it;
}


bool skiplist_iterator_end(SkipListIterator it) {
	return it->current == it->collection->sentinel;
}


SkipListIterator skiplist_iterator_next(SkipListIterator it) {
	it->current = it->next(it->current);
	return it;
}


int skiplist_iterator_value(SkipListIterator it) {
	return it->current->value;
}
