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

/* Définition de la structure SubList représentant de manière minimale une liste doublement chaînée sans sentinelle. 
 * Aucun élément ne précède la tête de la liste, de même qu'aucun élément ne suit la queue de la liste.
 */
typedef struct s_SubList {
	LinkedElement* head;
	LinkedElement* tail;
} SubList;


/*-----------------------------------------------------------------*/

/* Déclaration, allocation et initialisation d'une liste doublement chaînée */
List* list_create() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL; /* Echec de l'allocation de la liste */
	l->sentinel = malloc(sizeof(LinkedElement));
	if (l->sentinel == NULL) return NULL; /* Echec de l'allocation de la sentinelle */
	l->sentinel->next = l->sentinel->previous = l->sentinel;
	l->size = 0;
	return l;
}

/*-----------------------------------------------------------------*/

/* Ajout de l'élément v en fin de liste */
List* list_push_back(List* l, int v) {
	LinkedElement *new = malloc(sizeof(LinkedElement));
	new->previous = l->sentinel->previous; /* L'élément précédant est la queue de la liste */
	new->next = l->sentinel; /* L'élément suivant est la sentinelle */
	new->value = v;
	new->previous->next = new; /* L'élément suivant la queue de la liste est celui ajouté */
	l->sentinel->previous = new; /* L'élément ajouté est la nouvelle queue de la liste*/
	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

/* Libère l'ensemble des ressources allouées pour le stockage de la liste l que l'on met ensuite à NULL */
void list_delete(ptrList *l) {
	if (*l != NULL) {
		LinkedElement *toRemove; /* Pointeur vers l'élément à supprimer et libérer */
		LinkedElement *current = (*l)->sentinel->next; /* Pointeur pour parcourir la liste */

		while (current != (*l)->sentinel) {
			toRemove = current; /* Mémorisation de l'élément à supprimer */
			current = current->next;
			free(toRemove); /* Libération de l'élément à supprimer */
		}

		free((*l)->sentinel); /* Libération de la sentinelle */
		free(*l); /* Libération de la liste */
		*l = NULL; /* On affecte NULl à la liste l */
	}  
}

/*-----------------------------------------------------------------*/

/* Ajout de l'élément v en tête de liste */
List* list_push_front(List* l, int v) {
	LinkedElement *new = malloc(sizeof(LinkedElement));
	new->previous = l->sentinel; /* L'élément précédant est la sentinelle */
	new->next = l->sentinel->next; /* L'élément suivant est la tête de liste */
	new->value = v;
	new->next->previous = new; /* l'élément précédant la tête est celui ajouté */
	l->sentinel->next = new; /* L'élément ajouté est la nouvelle tête de liste */
	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

/* Renvoie la valeur de l'élément se trouvant en tête de liste */
int list_front(const List* l) {
	assert(!list_is_empty(l)); /* Pré-condition */
	return l->sentinel->next->value;
}

/*-----------------------------------------------------------------*/

/* Renvoie la valeur de lélément se trouvant en fin de liste */
int list_back(const List* l) {
	assert(!list_is_empty(l)); /* Pré-condition */
	return l->sentinel->previous->value;
}

/*-----------------------------------------------------------------*/

/* Supprime l'élément se trouvant en tête de la liste l */
List* list_pop_front(List* l) {
	assert(!list_is_empty(l)); /* Pré-condition */
	LinkedElement *toRemove = l->sentinel->next; /* Mémorisation de la tête de liste */
	toRemove->next->previous = l->sentinel; /* L'élément suivant la tête pointe vers la sentinelle */
	l->sentinel->next = toRemove->next; /* La sentinelle pointe vers la nouvelle tête */
	free(toRemove); /* Libération de l'ancienne tête */
	--(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

/* Supprime l'élément se trouvant en fin de la liste l */
List* list_pop_back(List* l){
	assert(!list_is_empty(l)); /* Pré-condition */
	LinkedElement *toRemove = l->sentinel->previous; /* Mémorisation de la queue de la liste */
	toRemove->previous->next = l->sentinel; /* l'élément précédant la queue pointe vers la sentinelle */
	l->sentinel->previous = toRemove->previous; /* La sentinelle pointe vers la nouvelle queue */
	free(toRemove); /* Libération de l'ancienne queue */
	--(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

/* Insère l'élément v à l'indice p dans la liste l */
List* list_insert_at(List* l, int p, int v) {
	assert(0 <= p && p <= l->size); /* Pré-condition */
	LinkedElement *current = l->sentinel->next; /* Pointeur pour parcourir la liste */
	LinkedElement *new = malloc(sizeof(LinkedElement));
	while (p--) current = current->next; /* Décalage jusqu'à la position souhaitée */
	/* Insertion du nouvel élément entre l'élément précédant current, et celui pointé par current */
	new->next = current;
	new->previous = current->previous; 
	new->value = v;
	current->previous->next = new;
	current->previous = new;
	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

/* Supprime l'élément se trouvant à l'indice p dans la liste l */
List* list_remove_at(List* l, int p) {
	assert(!list_is_empty(l) && 0 <= p && p <= l->size); /* Pré-condition */
	LinkedElement *toRemove;
	LinkedElement *current = l->sentinel->next; /* Pointeur pour parcourir la liste */
	while (p--) current = current->next; /* Décalage jusqu'à la position souhaitée */
	toRemove = current; /* Mémorisation de l'élément à supprimer */
	/* Redirection des liens pointants vers l'élément à supprimer */
	current->previous->next = current->next;
	current->next->previous = current->previous;
	--(l->size);
	free(toRemove); /* Libération de l'élément */
	return l;
}

/*-----------------------------------------------------------------*/

/* Renvoie la valeur de l'élément se trouvant à l'indice p dans la liste l */
int list_at(const List* l, int p) {
	assert(!list_is_empty(l) && 0 <= p && p <= l->size); /* Pré-condition */
	LinkedElement *current = l->sentinel->next; /* Pointeur pour parcourir la liste */
	while (p--) current = current->next; /* Décalage jusqu'à la position souhaitée */
	return current->value;
}

/*-----------------------------------------------------------------*/

/* Test si la liste l est vide */
bool list_is_empty(const List* l) {
	return l->size == 0;
}

/*-----------------------------------------------------------------*/

/* RenvoiE le nombre d'éléments que contient la liste l */
int list_size(const List* l) {
	return l->size;
}

/*-----------------------------------------------------------------*/

/* Remplace la valeur de chaque élément de la liste l
 * par la valeur de retour de la fonction f appliquée à cet élément 
 */
List* list_map(List* l, SimpleFunctor f) {
	for (LinkedElement *e = l->sentinel->next; e != l->sentinel; e = e->next) {
		e->value = f(e->value);
	}
	return l;
}

/* Exécute la fonction f, munie de l'environnement utilisateur userData, sur chaque élément de la liste l */
List* list_reduce(List* l, ReduceFunctor f, void *userData) {
	for (LinkedElement *e = l->sentinel->next; e != l->sentinel; e = e->next) {
		f(e->value, userData);
	}
	return l;
}

/*-----------------------------------------------------------------*/

/* Découpe une sous-liste l en deux nouvelles sous-listes de tailles égale à 1 élément près */
SubList list_split(SubList l) {
	SubList result = l;
	/* Le pointeur fast va avancer deux fois plus vite que le pointeur slow */
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
	/* On mémorise dans notre sous-liste result la cellule du milieu et celle qui suit */
	result.head = slowPointer;
	result.tail = slowPointer->next;
	/* On "casse" les liens du milieu pour réellement diviser la sous-liste en deux */
	slowPointer->next->previous = NULL;
	slowPointer->next = NULL;
	return result;
}

/*-----------------------------------------------------------------*/

/* Fusionne deux sous-listes triées leftlist et rightlist en respectant l'ordre défini par la fonction f */
SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
	SubList result;
	/* Premier test permettant de contruire notre sous-liste result avec la plus petite valeur en tête */
	if (f(leftlist.head->value, rightlist.head->value)) {
		result.head = leftlist.head; /* L'élément suivant de notre sous-liste result est la tête de la sous-liste gauche */
		leftlist.head = leftlist.head->next; /* Décalage de la tête de la sous-liste gauche */
	} else {
		result.head = rightlist.head;
		rightlist.head = rightlist.head->next;
	}

	LinkedElement *tmp = result.head; /* Pointeur permettant de construire la sous-liste result à partir de sa tête */
	tmp->previous = NULL;

	while (leftlist.head != NULL || rightlist.head != NULL) {
		/* On boucle tant que l'on a pas atteint la fin de l'une des deux sous-listes gauche ou droite */
		if (!rightlist.head || (leftlist.head && f(leftlist.head->value, rightlist.head->value))) {
			/* la fin de la sous-liste droite est atteinte, ou bien la tête de la sous-liste gauche est prioritaire */
			tmp->next = leftlist.head; 
			leftlist.head = leftlist.head->next;
		} else {
			/* la fin de la sous-liste gauche est atteinte, ou bien la tête de celle de droite est prioritaire */
			tmp->next = rightlist.head;
			rightlist.head = rightlist.head->next;
		}
		tmp->next->previous = tmp; /* On respecte la structure doublement chaînée de la sous-liste */
		tmp = tmp->next;
	}
	result.tail = tmp; /* Le dernier élément ajouté est la queue de la sous-liste */
	tmp->next = NULL;

	return result;
}

/*-----------------------------------------------------------------*/

/* Trie de façon récursive la sous-liste l selon la relation d'ordre définie par la fonction f */
SubList list_mergesort(SubList l, OrderFunctor f) {
	if (l.head == NULL || l.head->next == NULL) {
		/* La sous-liste est vide ou ne contient qu'un seul élément, elle est donc déjà triée */
		return l;
	} else {
		SubList leftlist, rightlist;
		SubList splitList = list_split(l); /* Découpage de la sous-liste l en deux nouvelles sous-listes */
		leftlist.head = l.head; /* La sous-liste de gauche commence au début de l */
		leftlist.tail = splitList.head; /* La sous-liste de gauche s'arrête au milieu de l */
		rightlist.head = splitList.tail; /* La sous-liste de droite commence juste après le milieu de l */
		rightlist.tail = l.tail; /* La sous-liste de droite s'arrête à la fin de l */
		/* Appels récursifs pour trier la sous-liste de départ */
		return list_merge(list_mergesort(leftlist, f), list_mergesort(rightlist, f), f);
	}
}

/*-----------------------------------------------------------------*/

/* Trie une liste doublement chaînée avec sentinelle selon l'ordre défini par la fonction f */
List* list_sort(List* l, OrderFunctor f) {
	SubList sl, result;
	/* "Conversion" d'une List en SubList sans sentinelle */
	sl.head = l->sentinel->next; /* Même tête que la liste l */
	sl.tail = l->sentinel->previous; /* Même queue que la liste l */
	sl.tail->next = NULL;
	sl.head->previous = NULL;
	/* Trie de la sous-liste obtenue et récupération du résultat */
	result = list_mergesort(sl, f);
	/* "Re-conversion" vers le type List avec remise en place de la sentinelle */
	l->sentinel->next = result.head; /* L'élément suivant la sentinelle est la tête de la liste */
	l->sentinel->previous = result.tail; /* L'élément précédant la sentinelle est la queue de la liste */
	result.head->previous = l->sentinel; /* L'élément précédant la tête est la sentinelle */
	result.tail->next = l->sentinel; /* L'élément suivant la queue est la sentinelle */
	return l;
}

