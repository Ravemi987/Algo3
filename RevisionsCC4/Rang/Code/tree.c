#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/*------------------------  TreeType  -----------------------------*/

#define LEFT 0
#define RIGHT 1
#define PARENT 2

struct _tree {
    // Links towards subtrees
    // Use constants LEFT, RIGHT and PARENT to access to corresponding subtree
    BinarySearchTree * links[3];
    // Key of the tree
    int key;
    // ranks
    int lrank; // rank in ascending order (number of keys less than the one of the node)
    int grank; // rank in decreasing order (number of keys greater than the one of the node)
};

/*------------------------  BaseTtree  -----------------------------*/

BinarySearchTree *tree_create() {
    return NULL;
}

BinarySearchTree *tree_cons(int key) {
    BinarySearchTree *t = malloc(sizeof(struct _tree));
    bzero(t->links, 3 * sizeof(BinarySearchTree *));
    t->key = key;
    t->lrank = t->grank = 0;
    return t;
}

bool tree_empty(const BinarySearchTree *t) {
    return t == NULL;
}

int tree_key(const BinarySearchTree *t) {
    assert(!tree_empty(t));
    return t->key;
}

BinarySearchTree *tree_left(const BinarySearchTree *t) {
    assert(!tree_empty(t));
    return t->links[LEFT];
}

BinarySearchTree *tree_right(const BinarySearchTree *t) {
    assert(!tree_empty(t));
    return t->links[RIGHT];
}

BinarySearchTree *tree_parent(const BinarySearchTree *t) {
    assert(!tree_empty(t));
    return t->links[PARENT];
}

int tree_lrank(const BinarySearchTree *t) {
    assert(!tree_empty(t));
    return t->lrank;
}

int tree_grank(const BinarySearchTree *t){
    assert(!tree_empty(t));
    return t->grank;
}

/*********************************************************************/
/**                        Control start here                       **/
/*********************************************************************/
/**
 *  Nom : Airiau              Prenom :  Rémi           Num Etud  : 22204545
 **/

/*
 *  Barème :  
 *    - Compilation sans warnings : 2 pts
 *    - Exercice 1 : 2 pts
 *    - Exercice 2 : 3 pts
 *    - Exercice 3 : 2 pts
 *    - Exercice 4 : 3 pts
 *    - Exercice 5 : 6 pts  
 *    - 2 points supplémentaires selon la qualité, clarté et concision du code.
 */

/* 
 *  Exercice 1 : Ecrire la fonction iterative d'ajout dans l'arbre
 */
void tree_add(BinarySearchTree **t, int k) {
    BinarySearchTree **current = t;
    BinarySearchTree *par = NULL;

    while (!tree_empty(*current)) {
        if (tree_key(*current) == k)
            return;
        par = *current;
        if ((*current)->key > k) {
            ((*current)->lrank)++;
            current = &((*current)->links[LEFT]);
        } else {
            ((*current)->grank)++;
            current = &((*current)->links[RIGHT]);
        }
    }

    *current = tree_cons(k);
    (*current)->links[PARENT] = par;
}

/*
 * Exercice 2 : Parcours prefixe iteratif pour affichage de l'arbre
 */
/* fonction iterative prefixe*/
void tree_depth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    BinarySearchTree *current = (BinarySearchTree*)t;
    BinarySearchTree *prev = t->links[PARENT];
    BinarySearchTree *next = t->links[PARENT];
    while(current != NULL) {
        if (tree_parent(current) == prev) {
            f(current, userData);
            prev = current;
            next = current->links[LEFT];
        }
        if (tree_empty(next) || prev == tree_left(current)) {
            prev = current;
            next = current->links[RIGHT];
        }
        if (tree_empty(next) || prev == tree_right(current)) {
            prev = current;
            next = current->links[PARENT];
        }
        current = next;
    }
}  

/*
 * Exercice 3 : Parcours infixes recursif dans l'ordre croissant et décroissant des clés
 */
/* fonction recursive dans l'ordre des clés*/
void tree_inorder(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (tree_empty(t))
        return;
    tree_inorder(t->links[LEFT], f, userData);
    f(t, userData);
    tree_inorder(t->links[RIGHT], f, userData);
}

/* fonction recursive dans l'ordre inverse des clés*/
void tree_reverseorder(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (tree_empty(t))
        return;
    tree_reverseorder(t->links[RIGHT], f, userData);
    f(t, userData);
    tree_reverseorder(t->links[LEFT], f, userData);
}

/* 
 * Exercice 4 : établissement de l'invariant de rang et programmation de l'opérateur retournant la taille de l'arbre
 */
int tree_size(const BinarySearchTree *t) {
    return t->lrank + t->grank + 1;
}

/* 
 * Exercice 5 : recherche d'une clé à partir de son rang.
 */

typedef struct _rank {
    BinarySearchTree *(*left)(const BinarySearchTree*);
    BinarySearchTree *(*right)(const BinarySearchTree*);
    int (*rank)(const BinarySearchTree *t);
} *RankAccessors;

int kth_element(const BinarySearchTree *t, int r, RankAccessors accessors) {
    if (t == NULL)
        return 0;
    int currentRank = (accessors->rank)(t);
    if (currentRank + 1 == r)
        return tree_key(t);
    else if (currentRank + 1 > r)
        return kth_element((accessors->left)(t), r, accessors);
    else
        return kth_element((accessors->right)(t), r-currentRank-1, accessors);
}

int tree_kth_lower(const BinarySearchTree *t, int p) {
    RankAccessors accessor = malloc(sizeof(struct _rank));
    accessor->left = tree_left;
    accessor->right = tree_right;
    accessor->rank = tree_lrank;
    int kth = kth_element(t, p, accessor);
    free(accessor);
    return kth;
}

int tree_kth_greater(const BinarySearchTree *t, int p) {
    RankAccessors accessor = malloc(sizeof(struct _rank));
    accessor->left = tree_right;
    accessor->right = tree_left;
    accessor->rank = tree_grank;
    int kth = kth_element(t, p, accessor);
    free(accessor);
    return kth;
}
