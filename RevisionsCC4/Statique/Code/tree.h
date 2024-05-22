/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Algorithmique 3
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Interface pour l'implantation du TAD StaticSearchTree.

 */
/*-----------------------------------------------------------------*/
#ifndef __STATIC_TREE__H__
#define __STATIC_TREE__H__
#include <stdbool.h>

/*------------------------  StaticSearchTree  -----------------------------*/
/*------------------------  Operateurs fournis  -----------------------------*/

/** Opaque definition of the type StaticSearchTree */
typedef struct _tree StaticSearchTree;

/** Constructor : builds an empty StaticSearchTree
 */
StaticSearchTree *tree_create();

/** Operator : is the tree empty ?
 * tree_empty : StaticSearchTree -> boolean
 */
bool tree_empty(const StaticSearchTree *t);

/** Operator : return the size (number of nodes) of the tree.
 * statictree_size : StaticSearchTree -> int
 */
int tree_size(const StaticSearchTree *t);

/** Operator : test if a node index is valid for the tree.
 * tree_isvalidnode : StaticSearchTree -> bool
 */
bool tree_isvalidnode(const StaticSearchTree *t, int node);

/** Operator : return the root node of the tree
 * @pre !tree_empty(t)
 */
int tree_rootnode(const StaticSearchTree *t);

/** Operator : returns the left subtree of the given node.
 * @pre tree_isvalidnode(t) 
 */
int tree_left(const StaticSearchTree *t, int node);

/** Operator : returns the right subtree of the given node.
 * @pre tree_isvalidnode(t) 
 */
int tree_right(const StaticSearchTree *t, int node);

/** Operator : returns the parent subtree of the given node.
 * @pre tree_isvalidnode(t) 
 */
int tree_parent(const StaticSearchTree *t, int node);

/** Operator : returns the value of the given node of the tree.
 * @pre tree_isvalidnode(t) 
 */
int tree_key(const StaticSearchTree *t, int node);

/** Functor with user data to be used with the several visitors that can run on trees.
 *  This functor receive as argument the tree, the node index that should be processed and an opaque
 *  pointer to user provided data.
 */
typedef void(*VisitFunctor)(const StaticSearchTree *, int nodeidx, void *);

/***************************************************************************************/
/**             Control requires implementing these functions                         **/
/***************************************************************************************/

/*
 * Exercice 1 : add a key on a binary search tree and map a functor on all nodes of the tree 
 */
/** Constructor : add a new value to a StaticSearchTree.
 * @param t the tree in which the new key will be added
 * @param k the key to add
 * @return the node id of the added node
 */
int tree_add(StaticSearchTree *t, int k);

/**
 * Apply a functor on each node of the tree, independantly from the tree topology.
 * @param t the tree to visit
 * @param f the functor to apply on visited nodes
 * @param userData opaque pointer to pass to the functor
 */
void tree_map_on_nodes(const StaticSearchTree *t, VisitFunctor f, void *userData);

/*
 * Exercice 2 : search for a keay in a binary search tree
 */
/**
 * search a key on a tree
 * @param t the tree to search in
 * @param k the key to search
 * @return the node index if the node is found, the value -1 if not
 */
int tree_search(const StaticSearchTree *t, int k);

/*------------------------  TreeVisitors  -----------------------------*/

/*
 * Exercice 3 : visit a tree in key increasing order and in key decreasing order
 */
/** inorder visitor
 * @param t the tree to visit
 * @param f the functor to apply on visited nodes
 * @param userData opaque pointer to pass to the functor
 */
void tree_inorder(const StaticSearchTree *t, VisitFunctor f, void *userData);

/** reverse order visitor 
 * @param t the tree to visit
 * @param f the functor to apply on visited nodes
 * @param userData opaque pointer to pass to the functor
 */
void tree_reverseorder(const StaticSearchTree *t, VisitFunctor f, void *userData);

/*
 * Exercice 4 : find the lowest common ancestor of two keys
 */
/**
 * Find the node id of the nearest common ancestor of two keys.
 * @param t the tree to visit
 * @param k1 the first key 
 * @param k2 the second key
 * @return the node index of the nearest common ancestor
 * @pre tree_search(t, k1) != -1 && tree_search(t, k2) != -1
 */
int tree_nearest_common_ancestor(const StaticSearchTree *t, int k1, int k2);


/* Exercice 5 : compute the distance (nu,ber of edge from one key to the other) between two keys */
/**
 * return the number of edges in the tree between two keys.
 * @param t the tree to visit
 * @param k1 the first key 
 * @param k2 the second key
 * @return the number of edges between keys
 * @pre tree_search(t, k1) != -1 && tree_search(t, k2) != -1
 */
int tree_distance(const StaticSearchTree *t, int k1, int k2);


#endif /* __STATIC_TREE__H__ */
