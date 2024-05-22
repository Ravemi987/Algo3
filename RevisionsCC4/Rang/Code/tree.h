/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Algorithmique 3
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Interface pour l'implantation du TAD BinarySearchTree.

 */
/*-----------------------------------------------------------------*/
#ifndef __TREE__H__
#define __TREE__H__
#include <stdbool.h>

/*------------------------  TreeType  -----------------------------*/

/** Opaque definition of the type BinaryTree */
typedef struct _tree BinarySearchTree;


/*------------------------  BaseTtree  -----------------------------*/

/** Constructor : builds an empty BinarySearchTree
 */
BinarySearchTree *tree_create();

/** Operator : is the tree empty ?
 * tree_empty : BinarySearchTree -> boolean
 */
bool tree_empty(const BinarySearchTree *t);

/** Operator : returns the key of the root node of the tree.
 * @pre !tree_empty(t)
 */
int tree_key(const BinarySearchTree *t);

/** Operator : returns the left subtree.
 * @pre !tree_empty(t)
 */
BinarySearchTree *tree_left(const BinarySearchTree *t);

/** Operator : returns the right subtree.
 * @pre !tree_empty(t)
 */
BinarySearchTree *tree_right(const BinarySearchTree *t);

/** Operator : returns the rank in ascending order of the tree root.
 * @pre !tree_empty(t)
 */
int tree_lrank(const BinarySearchTree *t);

/** Operator : returns the rank in descending order of the tree root.
 * @pre !tree_empty(t)
 */
int tree_grank(const BinarySearchTree *t);

/***************************************************************************************/
/**             Control requires implementing these functions                         **/
/***************************************************************************************/

/*------------------------  TreeDictionary  -----------------------------*/

/** Constructor : add a new key to the BinarySearchTree. */
/*
 * Exercice 1 : add a key according to binary search tree properties
 * Exercice 4 : establish ranks invariant of tree nodes
 */
void tree_add(BinarySearchTree **t, int k);

/*
 * Exercice 2 :  iterative, depth first prefix tree visitor 
 */
/** Functor with user data to be used with the several visitors that can run on trees.
 *  This functor receive as argument subtree from which the root node must be processes and an opaque
 *  pointer to user provided data.
 */
typedef void(*OperateFunctor)(const BinarySearchTree *, void *);

/** Depth first, prefix visitor, iterative */
void tree_depth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData);

/*
 * Exercice 3 :  recursive, depth first inorder and reverse order visitors
 */
/** In order visitor, recursive */
void tree_inorder(const BinarySearchTree *t, OperateFunctor f, void *userData);
/** Reverse order visitor, recursive */
void tree_reverseorder(const BinarySearchTree *t, OperateFunctor f, void *userData);

/* 
 * Exercice 4 : establish ranks and implement size operator
 */
int tree_size(const BinarySearchTree *t);

/* 
 * Exercice 5 : search for a node given its position p
 */
int tree_kth_greater(const BinarySearchTree *t, int p);
int tree_kth_lower(const BinarySearchTree *t, int p);


#endif /* __tree__H__ */
