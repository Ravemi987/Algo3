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

void split(Node *head, Node **a, Node **b) {
	Node* slow = head;
    Node* fast = head->next;
 
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
	
	*a = head;
    *b = slow->next;
    slow->next = NULL;
}

Node *merge(Node *a, Node *b) {
	if (a == NULL)
		return b;
	else if (b == NULL)
		return a;
	else {
		if (a->value <= b->value) {
			a->next = merge(a->next, b);
			a->next->previous = a;
			a->previous = NULL;
			return a;
		} 
		else {
			b->next = merge(a, b->next);
			b->next->previous = b;
			b->previous = NULL;
			return b;
		}
	}
}

Node* mergeSort(Node **head) {
	if (*head == NULL || (*head)->next == NULL)
		return *head;
	else {
		Node *a = NULL, *b = NULL;
		split(*head, &a, &b);
		return merge(mergeSort(&a), mergeSort(&b));
	}
}

int main() {

	List* l = list();

	l = push_back(l, 4);
	l = push_back(l, 3);
	l = push_back(l, 17);
	l = push_back(l, 48);
	l = push_back(l, 12);
	l = push_back(l, 24);
	l = push_back(l, 7);

	Node *start = mergeSort(&l->head);
	for (Node *current = start; current != NULL; current = current->next) {
		printf("%d ", current->value);
	}
	printf("\n");

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
