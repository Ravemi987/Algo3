#include "btree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


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

typedef int (*TreeMapFunctor)(int);


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

//Nouvelles implémentations
//  Binary search tree : renvoi 0 si l'arbre n'est pas vide 
bool btree_empty( BinaryTree *t) {
	return t == NULL;
}




//implémentation récursive de add
BinaryTree * bstree_add_rec(BinaryTree *t,int v){
	if(t == NULL){
		return t = btree_cons(btree_create(),btree_create(),v);
	}else {

		if(v < t->root) {
			t->left = bstree_add_rec(&(*t->left),v);
			t->left->parent = t;
		}
		else if (v > t->root){
			t->right = bstree_add_rec(&(*t->right),v);
			t->right->parent = t;
		}
	}
		
	return t;
}


//implémentation itérative de add
BinaryTree *bstree_add_ite(BinaryTree *t, int v) {
	if (t == NULL) {
		return t = btree_cons(btree_create(), btree_create(), v);
	} else {
		BinaryTree *tmp = t;
		while (tmp != NULL) {
			if (v < tmp->root) {
				if (tmp->left == NULL) {
					tmp->left = btree_cons(btree_create(), btree_create(), v);
					tmp->left->parent = tmp;
					tmp->parent = t;
				}
			} else if (v > tmp->root ){
				if (tmp->right == NULL) {
					tmp->right = btree_cons(btree_create(), btree_cons(), v);
					tmp->right->parent = tmp;
					tmp->parent = t;
				}
			} else {
				return NULL;
			}
		}
	}
}




//version récursive de search 
bool bstree_search_rec( BinaryTree *t, int v) {
	if(btree_empty(t)==1) //Arbre vide
		return false;
	else if (root(t)==v) 
		return true;      //valeure trouvée
	else if (v < root(t) && !btree_empty(t->left)  ) 
		return (!btree_empty(left(t)) && bstree_search_rec(left(t),v));
	else if  (v > root(t) && !btree_empty(t->right)  ) 
		return (!btree_empty(right(t)) && bstree_search_rec(right(t),v));
	else return false; 
}

//version itérative de search
bool bstree_search_iter(BinaryTree *t, int v){
	while(!btree_empty(t)){
		if(v < root(t) ){
			if(btree_empty(t->left)) return false;
			t = left(t);
		}
		else if(v > root(t) ){
			if(btree_empty(t->right)) return false;
			t = right(t);
		}
		else break;	
	}
	return !btree_empty(t);
}

//Successeur d'un noeud
// revoie le Nœud y dont la clé est la plus grande qui soit inférieure à la clé de x
BinaryTree * sucessor(BinaryTree *x){
	BinaryTree *y = x->right;
	if(y){
		while(y->left) y = y->left;
	}else {
		y = x->parent;
		while(y && (x== y->right)){
			x = y;
			y = y->parent;
		}
	}
	return(y);
}


//Prédécésseur d'un noeud
BinaryTree * predecessor(BinaryTree *x){
	BinaryTree *y = x->left;
	if(y){
		while(y->right) y = y->right;
	}else {
		y = x->parent;
		while(y && (x== y->left)){
			x = y;
			y = y->parent;
		}
	}
	return(y);
}


//suppression d'un noeud
//TODO






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
	
	printf("Session 3 \n");
	printf("%d\n",btree_empty(b_3));

	
	BinaryTree  *bin_tree = bstree_add_rec(NULL, 22);
	bin_tree = bstree_add_rec(bin_tree , 7);
	bin_tree = bstree_add_rec(bin_tree , 29);
	bin_tree = bstree_add_rec(bin_tree , 17);
	bin_tree = bstree_add_rec(bin_tree , 25);
	bin_tree = bstree_add_rec(bin_tree , 32);
	bin_tree = bstree_add_rec(bin_tree , 9);
	bin_tree = bstree_add_rec(bin_tree , 21);
	bin_tree = bstree_add_rec(bin_tree , 23);
	bin_tree = bstree_add_rec(bin_tree , 28);
	bin_tree = bstree_add_rec(bin_tree , 30);
	bin_tree = bstree_add_rec(bin_tree , 8);
	bin_tree = bstree_add_rec(bin_tree , 16);
	bin_tree = bstree_add_rec(bin_tree , 27);
	bin_tree = bstree_add_rec(bin_tree , 26);

	depth_prefix(bin_tree , print_i);
	//depth_postfix(bin_tree , print_i);
	printf("vide ? %d\n",btree_empty(bin_tree) );

 	printf("Recherche récursive de 23 : %d \n",bstree_search_rec(bin_tree,23));
 	printf("Recherche récursive de 40 %d \n",bstree_search_rec(bin_tree,40));

 

	printf("Successeur de 22 ? %d\n",sucessor(bin_tree)->root );

	//TODO 
	//Ajouter les fonctions manquantes 


	return 0;
}