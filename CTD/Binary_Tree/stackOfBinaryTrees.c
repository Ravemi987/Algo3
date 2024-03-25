#include "btree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

bool tree_empty (const BinaryTree *t);

struct _btree {
	BinaryTree * parent;
	BinaryTree * left;
	BinaryTree * right;
	int root;
};

BinaryTree *btree_create(){
	return NULL;
}



BinaryTree *btree_cons(BinaryTree *left, BinaryTree *right, int root){
	BinaryTree *t = malloc(sizeof(BinaryTree));
	t->parent = NULL;

	t->right = right;
	if(t->right)t->right->parent = t;

	t->left = left;
	if(t->left) t->left->parent = t;

	t-> root = root;
	return t;
}

int root(BinaryTree *t){
	
	return(t->root);
}

BinaryTree *parent(BinaryTree *t){
	assert(t->parent!=NULL);
	return t->parent;
}

BinaryTree *left(BinaryTree *t){
	assert(t->left!=NULL);
	return t->left;
}
 
BinaryTree *right(BinaryTree *t){
		assert(t->right!=NULL);
	return t->right;
}

//Session 1

//http://math.univ-lyon1.fr/irem/IMG/pdf/parcours_arbre_avec_solutions-2.pdf
//l’ordre infixe : on liste chaque sommet ayant un fils gauche la seconde 
//fois qu’on le voit et chaquesommet sans fils gauche la première fois qu’on 
//le voit. Ce qui donne ici : . . 

typedef int (*TreeMapFunctor)(int);
BinaryTree *map(BinaryTree *l,TreeMapFunctor f);

void depth_prefix(BinaryTree *t, TreeMapFunctor f){
	if(t==NULL) return;
  f(t->root);
  depth_prefix(t->left,f);
  depth_prefix(t->right,f);
}

void depth_infix(BinaryTree *t, TreeMapFunctor f){
	if(t==NULL) return;

	depth_infix(t->left, f);
	f(t->root);
	depth_infix(t->right, f);
}

void depth_postfix(BinaryTree *t, TreeMapFunctor f){
	if(t==NULL) return;

 depth_postfix(t->left, f);
 depth_postfix(t->right, f);
 f(t->root);
}

//affichage da valeur 
int print_i(int i){
	printf("%d\n", i);
	return i;
}

//Session Liste + Arbre
// Ajout d'une liste chainée pour gérer des arbres
typedef struct _List List;
typedef struct _Maillon Maillon;

 struct _Maillon  {
	BinaryTree *value;
	struct _Maillon  *previous;
	struct _Maillon  *next;
};

struct _List {

	 Maillon *head;
	 Maillon *tail;
	 unsigned int size;

};


//Creation d’une liste doublement chaˆın ́ee vide
List  *list(){
	List *l = malloc(sizeof(List));
	l->head = l->tail = NULL;
	l->size = 0;
	return l;
}


//Operateurs d’ajout et de suppression d’un élément à une liste doublement chaînée
List *push_back(List *l, BinaryTree * v){
	Maillon **insert_at = (l->size ? &(l->tail->next) : &(l->head));
	Maillon *new = malloc(sizeof(Maillon));
	new -> value = v;
	new-> next = NULL;
	*insert_at = new;
	l->tail = new;
	++(l->size);
	return(l);

}


//Operateurs d’ajout et de suppression d’un élément à une liste doublement chaînée
List *push_front(List *l, BinaryTree * v){
	Maillon *new = malloc(sizeof(Maillon));
	new -> value = v;
	new-> next = l->head;
	new->previous = NULL;
	l->head->previous = new;
	l->head = new;
	++(l->size);
	return(l);

}


//Opérateurs d'état du TAD Liste
bool list_empty (const List *l){
	return l->size == 0;
}


List *pop_front(List *l){
	assert(!list_empty(l));	
	Maillon *e;
	e = l->head;
	l->head = e->next;
		if(l->head) l->head->previous = NULL;
		else l->tail = l->head;
	--(l->size);
	free(e);
	return(l);
}


BinaryTree * front(const List *l){
	assert(!list_empty(l));
	return l->head->value ;
}

BinaryTree * back(const List *l){
	assert(!list_empty(l));
	return l->tail->value ;
}





//Implantation itérative de parcours de prefix
void stack_infix(BinaryTree *t , TreeMapFunctor f){
		List* l = list();
		
		BinaryTree *temp_tree = t;
	
		l = push_back(l,temp_tree);
		

		
		while(!list_empty(l)){
		temp_tree=l->head->value;
		
		// affichage du noeud 
		f(root(temp_tree));
		
		printf("\n");
		
		//ajout des enfants de t
		if(temp_tree->left) push_back(l,temp_tree->left);
		
		if(temp_tree->right) push_back(l,temp_tree->right);
		
		//suppression de l'élément en tête
		l = pop_front(l);
		
		}
	


		
		//TODO : stack le reste
}


// Session 5

//Opérateurs d'état du TAD  BinaryTree
bool tree_empty (const BinaryTree *t){
	return t == NULL;
}

/*
void iterative_depth_infix(BinaryTree *t, TreeMapFunctor f){
	BinaryTree *current = t;
	BinaryTree *next = parent(t);
	BinaryTree *prev = parent(t);

	while(t!=NULL){


		/*if(prev == parent(current)){
			prev = current;
			next = left(current);
		}
		if(tree_empty(next)|| prev == left(current)){
			f(root(current));
			prev = current;
			next = right(current);
		}
		if(tree_empty(next) || prev == right(current)){
			prev = current;
			next = parent(current);
		}
	//	current = next;
	}
  }*/

int main(){


	printf("Session 1 \n" );


	BinaryTree  *b_11_l = btree_cons(btree_create(),btree_create(),11);
	BinaryTree  *b_12_r = btree_cons(btree_create(),btree_create(),12);
	BinaryTree *b_31 = btree_cons(b_11_l,b_12_r,3);

	BinaryTree  *b_21_l = btree_cons(btree_create(),btree_create(),21);
	BinaryTree  *b_22_r = btree_cons(btree_create(),btree_create(),22);
	BinaryTree *b_32 = btree_cons(b_21_l ,b_22_r,4);

	printf("%d\n", b_31->left->root);
	printf("%d\n", b_31->right->root);
	printf("%d\n", b_31->root);
	printf("fusion\n");
	BinaryTree *b_3 = btree_cons(b_31,b_32,100);
	printf("%d\n", root(b_3));
	printf("%d\n", root(left(b_3)));
	printf("%d\n", root(left(left(b_3))));


	printf("Session 2 \n" );
	printf("Prefix:\n");
	depth_prefix(b_3, print_i);
	printf("\nInfix:\n");
	depth_infix(b_3, print_i);
	printf("\nPostfix:\n");
	depth_postfix(b_3, print_i);



	printf("Session 3\n");
	List* l = list();
	
	l = push_back(l,b_3);
	l = push_back(l,b_32);
	l = push_back(l,b_21_l);
	
	printf("Size %d\n",l->size );
	printf("Head %d\n",root(l->head->value) );
	printf("Tail %d\n",root(l->tail->value) );
	
	printf("Push et pop front \n");
	
	l = push_front(l,b_32);

	
	printf("Size %d\n",l->size );
	printf("Head %d\n",root(l->head->value) );
	printf("Tail %d\n",root(l->tail->value) );
	
	l = pop_front(l);

	printf("Size %d\n",l->size );
	printf("Head %d\n",root(l->head->value) );
	printf("Tail %d\n",root(l->tail->value) );

	printf("list empty %d \n",list_empty(l));
	l = pop_front(l);
	l = pop_front(l);
	l = pop_front(l);
	printf("list empty %d \n",list_empty(l));


	//printf("Affichage avec stack_tree front \n");


	stack_infix(b_3, print_i);




	return 0;
}
