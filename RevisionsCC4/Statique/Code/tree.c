#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------  StaticSearchTree  -----------------------------*/
#define LEFT 0
#define RIGHT 1
#define PARENT 2

typedef struct _node {
    // Key of the node
    int key;
    // Links towards subtrees
    // Use constants LEFT, RIGHT and PARENT to access to corresponding subtree
    int links[3];
} Node;

struct _tree {
    // number of nodes in the tree
    int size;
    // node capacity (max number of node before reallocation)
    int capacity;
    // index of the root node
    int root;
    Node* tabNodes;
};

// ----- management of the node array ----- //
bool tree_reserve(StaticSearchTree *tree, int capacity) {
    Node* newnodes = realloc( tree->tabNodes, sizeof(struct _node) * capacity );
    if ( newnodes == NULL ) {
        return false;
    }
    tree->tabNodes = newnodes;
    tree->capacity = capacity;
    return true;
}

// Note due to realloc, if the link pointer is an address into the node array then, after this call, the link pointer may be invalid.
int tree_add_node(StaticSearchTree *tree, int key, int parent, int* link) {
    // Store the new node index into the given link
    *link = tree->size;
    // Check size and reallocate if needed
    if(tree->size == tree->capacity) {
        if ( !tree_reserve(tree, tree->capacity*2) ) {
            return -1;
        };
    }
    // store the node
    tree->tabNodes[tree->size].key = key;
    tree->tabNodes[tree->size].links[PARENT] = parent;
    tree->tabNodes[tree->size].links[LEFT] = tree->tabNodes[tree->size].links[RIGHT] =  -1;
    return tree->size++;
}

/** Constructor : builds an empty StaticSearchTree
 */
StaticSearchTree *tree_create() {
    StaticSearchTree* tree = malloc( sizeof(struct _tree) );
    tree->size = 0;
    tree->root = -1;
    tree->tabNodes = NULL;
    tree_reserve(tree, 1);
    return tree;
}

/** Operator : is the tree empty ?
 * tree_empty : StaticSearchTree -> boolean
 */
bool tree_empty(const StaticSearchTree *t) {
    return t->size == 0;
}

/** Operator : return the size (number of nodes) of the tree.
 * statictree_size : StaticSearchTree -> int
 */
int tree_size(const StaticSearchTree *t) {
    return t->size;
}

/** Operator : test if a node index is valid for the tree.
 * tree_isvalidnode : StaticSearchTree -> bool
 */
bool tree_isvalidnode(const StaticSearchTree *t, int node) {
    return node >= 0 && node < t->size;
}

/** Operator : return the root node of the tree
 * @pre !tree_empty(t)
 */
int tree_rootnode(const StaticSearchTree *t) {
    assert ( !tree_empty(t) );
    return t->root;
}

/** Operator : returns the left subtree of the given node.
 * @pre tree_isvalidnode(t) 
 */
int tree_left(const StaticSearchTree *t, int node) {
    assert (tree_isvalidnode(t, node));
    return t->tabNodes[node].links[LEFT];
}

/** Operator : returns the right subtree of the given node.
 * @pre tree_isvalidnode(t) 
 */
int tree_right(const StaticSearchTree *t, int node) {
    assert (tree_isvalidnode(t, node));
    return t->tabNodes[node].links[RIGHT];
}

/** Operator : returns the parent subtree of the given node.
 * @pre tree_isvalidnode(t) 
 */
int tree_parent(const StaticSearchTree *t, int node) {
    assert (tree_isvalidnode(t, node));
    return t->tabNodes[node].links[PARENT];
}

/** Operator : returns the value of the given node of the tree.
 * @pre tree_isvalidnode(t) 
 */
int tree_key(const StaticSearchTree *t, int node) {
    assert (tree_isvalidnode(t, node));
    return t->tabNodes[node].key;
}

/*********************************************************************/
/**                        Control start here                       **/
/*********************************************************************/
/**
 *  Nom : Airiau               Prenom :  Rémi             Num Etud  : 25
 **/

/*
 *  Barème :  
 *    - Compilation sans warnings : 2 pts
 *    - Exercice 1 : 2 pts
 *    - Exercice 2 : 2 pts
 *    - Exercice 3 : 3 pts
 *    - Exercice 4 : 5 pts
 *    - Exercice 5 : 4 pts  
 *    - 2 points supplémentaires selon la qualité, clarté et concision du code.
 */

/* x
 * Exercice 1 : implantation iterative du constructeur
 */
/** Constructor : add a new value to a StaticSearchTree.
 * @param t the tree in which the new key will be added
 * @param k the key to add
 * @return the node id of the added node
 */
int tree_add(StaticSearchTree *t, int k) {
    int *current = &(t->root);
    int par = -1;
    while (*current != -1) {
        if ((t->tabNodes[*current]).key == k) {
            return *current;
        }
        par = *current;
        current = (t->tabNodes[*current]).key > k ? &(t->tabNodes[*current].links[LEFT]) : & (t->tabNodes[*current].links[RIGHT]);
    }
    return tree_add_node(t, k, par, current);
}

/*
 * Exercice 1 : application d'un foncteur sur tous les noeuds map sans tenir compte de la topologie
 */
/**
 * Apply a functor on each node of the tree, independantly from the tree topology.
 * @param t the tree to visit
 * @param f the functor to apply on visited nodes
 * @param userData opaque pointer to pass to the functor
 */
void tree_map_on_nodes(const StaticSearchTree *t, VisitFunctor f, void *userData) {
    for (int i = t->root; i < t->size; i++)
        f(t, i, userData);
}

/*
 * Exercice 2 : Recherche recursive d'une cle dans un arbre
 */

/**
 * search a key on a tree
 * @param t the tree to search in
 * @param k the key to search
 * @return the node index if the node is found, the value -1 if not
 */
int tree_search(const StaticSearchTree *t, int k){
    StaticSearchTree tmp = *t;
    if (tmp.root == -1) {
        return -1;
    } else if (tmp.tabNodes[tmp.root].key == k) {
        return tmp.root;
    } else if (tmp.tabNodes[tmp.root].key > k) {
        tmp.root = tmp.tabNodes[tmp.root].links[LEFT];
        return tree_search(&tmp, k);
    } else {
        tmp.root = tmp.tabNodes[tmp.root].links[RIGHT];
        return tree_search(&tmp, k);
    }
}

/* 
 * Exerice 3 : visite iterative dans l'ordre et dans l'ordre inverse avec programmation d'ordre supérieur
 */

/* Accessors and function helper */

typedef struct _accessor {
    int (*first)(const StaticSearchTree *, int);
    int (*second)(const StaticSearchTree *, int);
    int (*third)(const StaticSearchTree *, int);
} *ChildAccessFunctors;


void tree_visit_infix(const StaticSearchTree*t, VisitFunctor f, void *userData, ChildAccessFunctors functors) {
    int current = t->root;
    int prev = functors->third(t, current);
    int next = functors->third(t, current);

    while(current != -1) {
        if (tree_parent(t, current) == prev) {
            prev = current;
            next = functors->first(t, current);
        }
        if (next == -1 || tree_left(t, current) == prev) {
            f(t, current, userData);
            prev = current;
            next = functors->second(t, current);
        }
        if (next == -1 || tree_right(t, current) == prev) {
            prev = current;
            next = functors->third(t, current);
        }
        current = next;
    }
}

/** inorder visitor
 * @param t the tree to visit
 * @param f the functor to apply on visited nodes
 * @param userData opaque pointer to pass to the functor
 */
void tree_inorder(const StaticSearchTree *t, VisitFunctor f, void *userData) {
    ChildAccessFunctors functor = malloc(sizeof(struct _accessor));
    functor->first = tree_left;
    functor->second = tree_right;
    functor->third = tree_parent,
    tree_visit_infix(t, f, userData, functor);
    free(functor);
}

/** reverse order visitor
 * @param t the tree to visit
 * @param f the functor to apply on visited nodes
 * @param userData opaque pointer to pass to the functor
 */
void tree_reverseorder(const StaticSearchTree *t, VisitFunctor f, void *userData) {
    ChildAccessFunctors functor = malloc(sizeof(struct _accessor));
    functor->first = tree_right;
    functor->second = tree_left;
    functor->third = tree_parent,
    tree_visit_infix(t, f, userData, functor);
    free(functor);
}

/*
 * Exercice 4 : recherche du plus petit ancetre commun
 */
/**
 * Find the node id of the nearest common ancestor of two keys.
 * @param t the tree to visit
 * @param k1 the first key 
 * @param k2 the second key
 * @return the node index of the nearest common ancestor
 * @pre tree_search(t, k1) != -1 && tree_search(t, k2) != -1
 */
int tree_nearest_common_ancestor(const StaticSearchTree *t, int k1, int k2) {
    assert(tree_search(t, k1) != -1 && tree_search(t, k2) != -1);
    StaticSearchTree tmp_left = *t;
    StaticSearchTree tmp_right = *t;
    int cur = t->root;
    while (cur != -1) {
        tmp_left.root = t->tabNodes[cur].links[LEFT];
        tmp_right.root = t->tabNodes[cur].links[RIGHT];
        if (t->tabNodes[cur].key == k1 || t->tabNodes[cur].key == k2) {
            break;
        } else if (((tree_search(&tmp_left, k1) != -1) && (tree_search(&tmp_right, k2)) != -1) || ((tree_search(&tmp_right, k1) != -1) && (tree_search(&tmp_left, k2)) != -1)) {
            break;
        } else {
            cur = ((tree_search(&tmp_left, k1) != -1)) ? tmp_left.root : tmp_right.root;
        }
    }
    return cur;
}

/* 
 * Exercice 5 : calcul de la distance entre deux cles de l'arbre (nombre d'arete dans l'arbre)
 */

int min(int k1, int k2) {
    if (k1 < k2)
        return k1;
    return k2;
}

int depth(const StaticSearchTree *t, int k) {
    StaticSearchTree tmp_left = *t;
    tmp_left.root = t->tabNodes[t->root].links[LEFT];
    StaticSearchTree tmp_right = *t;
    tmp_right.root = t->tabNodes[t->root].links[RIGHT];

    if (t->root == -1 || t->tabNodes[t->root].key == k) {
        return 0;
    } else {
        return 1 + min(depth(&tmp_left, k), depth(&tmp_right, k));
    }
}

/**
 * return the number of edges in the tree between two keys.
 * @param t the tree to visit
 * @param k1 the first key 
 * @param k2 the second key
 * @return the number of edges between keys
 * @pre tree_search(t, k1) != -1 && tree_search(t, k2) != -1
 */
int tree_distance(const StaticSearchTree *t, int k1, int k2) {
    if ((tree_search(t, k1) == -1) && (tree_search(t, k2) == -1)) {
        return -1;
    }
    else {
        int nca = tree_nearest_common_ancestor(t, k1, k2);
        StaticSearchTree tmp = *t;
        tmp.root = nca;
        return depth(&tmp, k1) + depth(&tmp, k2);
    }
}
