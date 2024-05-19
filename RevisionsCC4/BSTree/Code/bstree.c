#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);
void bstree_free_node(const BinarySearchTree *t, void *userData);

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

void bstree_free_node(const BinarySearchTree *t, void *userData) {
    (void) userData;
    free((BinarySearchTree *)t);
}

void bstree_delete(ptrBinarySearchTree *t) {
    bstree_depth_postfix(*t, bstree_free_node, NULL);
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

    BinarySearchTree *cur = x->right;
    if (cur) {
        while (cur->left)
            cur = cur->left;
    } else {
        cur = x->parent;
        while(cur && (x == cur->right)) {
            x = cur;
            cur = cur->parent;
        }
    }
    return cur;
}

BinarySearchTree *bstree_predecessor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));

    BinarySearchTree *cur = x->left;
    if (cur) {
        while (cur->right)
            cur = cur->right;
    } else {
        cur = x->parent;
        while (cur && (x == cur->left)) {
            x = cur;
            cur = cur->parent;
        }
    }
    return cur;
}

void bstree_swap_nodes(ptrBinarySearchTree *tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));

    if (from == to)
        return;

    ptrBinarySearchTree fromLeft = from->left;
    ptrBinarySearchTree fromRight = from->right;
    ptrBinarySearchTree toLeft = to->left;
    ptrBinarySearchTree toRight = to->right;

    if (fromLeft) fromLeft->parent = to;
    if (fromRight) fromRight->parent = to;
    if(toLeft) toLeft->parent = from;
    if(toRight) toRight->parent = from;

    to->left = fromLeft;
    to->right = fromRight;
    from->left = toLeft;
    from->right = toRight;
    
    if (from->parent) {
        if (from->parent->left == from) {
            from->parent->left = to;
        } else {
            from->parent->right = to;
        }
    } else {
        *tree = to;
    }

    if (to->parent) {
        if (to->parent->left == to) {
            to->parent->left = from;
        } else {
            to->parent->right = from;
        }
    } else {
        *tree = from;
    }

    ptrBinarySearchTree temp = from->parent;
    from->parent = to->parent;
    to->parent = temp;
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

    while (current && current->root != v)
        current = (current->root > v) ? current->left : current->right;
    if (current)
        bstree_remove_node(t, current);
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
    BinarySearchTree *current = (BinarySearchTree*)t;
    BinarySearchTree *prev = t->parent;
    BinarySearchTree *next = t->parent;

    while (!bstree_empty(current)) {
        if (current->parent == prev) {
            prev = current; next = current->left;
        }
        if (bstree_empty(next) || current->left == prev) {
            f(current, userData);
            prev = current; next = current->right;
        }
        if (bstree_empty(next) || current->right == prev) {
            prev = current; next = current->parent;
        }
        current = next;
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
    const BinarySearchTree *cur = e->left;
    if (cur)
        while (cur->left)
            cur = cur->left;
    return cur;
}

/* maximum element of the collection */
const BinarySearchTree *goto_max(const BinarySearchTree *e) {
	const BinarySearchTree *cur = e->right;
    if (cur)
        while (cur->right)
            cur = cur->right;
    return cur;
}

/* constructor */
BSTreeIterator *bstree_iterator_create(const BinarySearchTree *collection, IteratorDirection direction) {
	BSTreeIterator *i = malloc(sizeof(BSTreeIterator));
    if (i == NULL) return NULL;

    i->collection = collection;

    if (direction == forward) {
        i->begin = goto_min;
        i->next = bstree_successor;
    } else {
        i->begin = goto_max;
        i->next = bstree_predecessor;
    }

    i->current = i->begin(collection);
    return i;
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

