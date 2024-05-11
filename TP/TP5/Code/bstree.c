#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);

/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree *parent;
    BinarySearchTree *left;
    BinarySearchTree *right;
    int root;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree *bstree_create(void) {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree *bstree_cons(BinarySearchTree *left, BinarySearchTree *right, int root) {
    BinarySearchTree *t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->left = left;
    t->right = right;
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->root = root;
    return t;
}

void bstree_delete(ptrBinarySearchTree *t) {
    while (!bstree_empty(*t))
        bstree_remove_node(t, *t);
}

bool bstree_empty(const BinarySearchTree *t) {
    return t == NULL;
}

int bstree_root(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->root;
}

BinarySearchTree *bstree_left(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree *bstree_right(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree *bstree_parent(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->parent;
}

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree *t, int v) {
	ptrBinarySearchTree *cur = t;
    BinarySearchTree *par = NULL;

    while (*cur != NULL) {
        par = *cur;
        if ((*cur)->root == v) {
            return;
        }
        cur = ((*cur)->root > v) ? &((*cur)->left) : &((*cur)->right);
    }

    *cur = bstree_cons(NULL, NULL, v);
    (*cur)->parent = par;
}

bool bstree_search(const BinarySearchTree *t, int v) {
    while (!bstree_empty(t)) {
        if (t->root == v)
            return true;
        t = (t->root > v) ? t->left : t->right;
    }
    return false;
}

BinarySearchTree *bstree_successor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));
    BinarySearchTree *cur = x;

    cur = cur->right;
    if (bstree_empty(cur)) return NULL;

    while (!bstree_empty(cur->left))
        cur = cur->left;

    return cur;;
}

BinarySearchTree *bstree_predecessor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));
    BinarySearchTree *cur = x;

    cur = cur->left;
    if (bstree_empty(cur)) return NULL;

    while (!bstree_empty(cur->right))
        cur = cur->right;

    return cur;
}

void bstree_swap_nodes(ptrBinarySearchTree *tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    ptrBinarySearchTree fromPar = from->parent;
    ptrBinarySearchTree toPar = to->parent;
    ptrBinarySearchTree fromSonLeft, fromSonRight, toSonLeft, toSonRight;
    if (bstree_empty(fromPar))
        fromPar = *tree;
    if (bstree_empty(toPar))
        toPar = *tree;

    fromSonLeft = from->left;
    fromSonRight =from->right;
    toSonLeft = to->left;
    toSonRight = to->right;

    if (!bstree_empty(fromSonLeft)) fromSonLeft->parent = to;
    if (!bstree_empty(fromSonRight)) fromSonRight->parent = to;
    to->left = fromSonLeft;
    to->right = fromSonRight;

    if(!bstree_empty(toSonLeft)) toSonLeft->parent = from;
    if(!bstree_empty(toSonRight)) toSonRight->parent = from;
    from->left = toSonLeft;
    from->right = toSonRight;
    
    if (from->parent->left == from) {
        from->parent->left = to;
    } else {
        from->parent->right = to;
    }
    from->parent = toPar;

    if (to->parent->left == to) {
        to->parent->left = from;
    } else {
        to->parent->right = from;
    }
    to->parent = fromPar;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));
    ptrBinarySearchTree *link;

    if (bstree_empty(current->parent)) {
        link = t;
    } else if (current->parent->left == current) {
        link = &(current->parent->left);
    } else {
        link = &(current->parent->right);
    }

    if (current->left == current->right) {
        *link = bstree_create();
        free(current);
    } else if (bstree_empty(current->left)) {
        *link = current->right;
        current->right->parent = current->parent;
        free(current);
    } else if (bstree_empty(current->right)) {
        *link = current->left;
        current->left->parent = current->parent;;
        free(current);
    } else {
        ptrBinarySearchTree leaf = bstree_successor(current);
        if (leaf) {
            bstree_swap_nodes(t, current, leaf);
            bstree_remove_node(t, current);
        }
    }
}

void bstree_remove(ptrBinarySearchTree *t, int v) {
    ptrBinarySearchTree current = *t;

    while (current) {
        if (current->root == v) {
            break;
        }
        current = (current->root > v) ? current->left : current->right;
    }

    if (current) {
        bstree_remove_node(t, current);
    }
}

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (bstree_empty(t)) {
        return;
    }
    f(t, userData);
    bstree_depth_prefix(t->left, f, userData);
    bstree_depth_prefix(t->right, f, userData);
}

void bstree_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (bstree_empty(t)) {
        return;
    }
    bstree_depth_infix(t->left, f, userData);
    f(t, userData);
    bstree_depth_infix(t->right, f, userData);
}

void bstree_depth_postfix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (bstree_empty(t)) {
        return;
    }
    bstree_depth_postfix(t->left, f, userData);
    bstree_depth_postfix(t->right, f, userData);
    f(t, userData);
}

void bstree_iterative_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    BinarySearchTree *current = t;
    BinarySearchTree *prev = t->parent;
    BinarySearchTree *next;

    if (current->parent == prev && !bstree_empty(current->left)) {
        current = current->left;
        prev = current;
    }
}

void bstree_iterative_breadth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    Queue *q = createQueue();

    q = queuePush(q, t);
    while (!queueEmpty(q)) {
        BinarySearchTree *cur = (BinarySearchTree*)queueTop(q);
        f(cur, userData);
        q = queuePop(q);
        if (!bstree_empty(cur->left))
            q = queuePush(q, cur->left);
        if (!bstree_empty(cur->right))
            q =queuePush(q, cur->right);
    }

    deleteQueue(&q);
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree *collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree *(*begin)(const BinarySearchTree *);
    /* the current element pointed by the iterator */
    const BinarySearchTree *current;
    /* function that goes to the next element according to the iterator direction */
    BinarySearchTree *(*next)(const BinarySearchTree *);
};

/* minimum element of the collection */
const BinarySearchTree *goto_min(const BinarySearchTree *e) {
	(void)e;
	return NULL;
}

/* maximum element of the collection */
const BinarySearchTree *goto_max(const BinarySearchTree *e) {
	(void)e;
	return NULL;
}

/* constructor */
BSTreeIterator *bstree_iterator_create(const BinarySearchTree *collection, IteratorDirection direction) {
	(void)collection; (void)direction;
	return NULL;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator *i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator *bstree_iterator_begin(BSTreeIterator *i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator *i) {
    return i->current == NULL;
}

BSTreeIterator *bstree_iterator_next(BSTreeIterator *i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree *bstree_iterator_value(const BSTreeIterator *i) {
    return i->current;
}

