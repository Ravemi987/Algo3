#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);
void bstree_free_node(const BinarySearchTree *t, void *userData);
void printNode(const BinarySearchTree *n, void *out);
void leftrotate(BinarySearchTree *x);
void rightrotate(BinarySearchTree *y);
ptrBinarySearchTree grandparent(ptrBinarySearchTree n);
ptrBinarySearchTree uncle(ptrBinarySearchTree n);
ptrBinarySearchTree fixredblack_insert(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case0(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case1(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2_left(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2_right(ptrBinarySearchTree x);

/*------------------------   RBT ENUM   -----------------------------*/

typedef enum {red, black} NodeColor;

/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree *parent;
    BinarySearchTree *left;
    BinarySearchTree *right;
    NodeColor color;
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
    t->color = red;
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

    BinarySearchTree *stop = fixredblack_insert(*cur);
    if (stop->parent == NULL)
        *t = stop;
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

/*------------------------  RedBlackTrees  -----------------------------*/

void printNode(const BinarySearchTree *n, void *out) {
    FILE *file = (FILE *) out;
    char *color = (n->color == red) ? "red" : "grey";

    printf("%d ", bstree_root(n));
    fprintf(file, "\tn%d [style=filled, fillcolor=%s, label=\"{{<parent>}|%d|{<left>|<right>}}\"];\n",
            bstree_root(n), color, bstree_root(n));

    if (bstree_left(n)) {
        fprintf(file, "\tn%d:left:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(bstree_left(n)));
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_root(n));
        fprintf(file, "\tn%d:left:c -> lnil%d:n [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(n));
    }
    if (bstree_right(n)) {
        fprintf(file, "\tn%d:right:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(bstree_right(n)));
    } else {
        fprintf(file, "\trnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_root(n));
        fprintf(file, "\tn%d:right:c -> rnil%d:n [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(n));
    }
}

void rbtree_export_dot(const BinarySearchTree *t, FILE *file) {
    fprintf(file, "digraph RedBlackTree {\n\tgraph [ranksep =0.5];\n\tnode [shape = record];\n\n");
    bstree_iterative_depth_infix(t, printNode, file);
    fprintf(file, "\n}\n");
}

void leftrotate(BinarySearchTree *x) {
    assert(!bstree_empty(x) && !bstree_empty(x->right));
    BinarySearchTree *gamma;
    BinarySearchTree *alpha;

    /* Memorisation de alpha et gamma */
    gamma = x->right->right;
    alpha = x->left;

    /* Deplacement de beta a la place de gamma */
    if (x->right->left)
        x->right->right = x->right->left;
    else
        x->right->right = NULL;

    /* Deplacement de alpha a l'ancienne place de beta */
    if (alpha) {
        x->right->left = alpha;
        alpha->parent = x->right;
    } else {
        x->right->left = NULL;
    }

    /* Deplacement de la branche de droite a gauche */
    x->left = x->right;

    /* Deplacement de gamma sur la branche de droite */
    if (gamma) {
        x->right = gamma;
        gamma->parent = x;
    } else {
        x->right = NULL;
    }

    /* Echange de x et y */
    bstree_swap_nodes(&x, x, x->left);
}

void rightrotate(BinarySearchTree *y) {
    assert(!bstree_empty(y) && !bstree_empty(y->left));
    BinarySearchTree *gamma;
    BinarySearchTree *alpha;

    /* Memorisation de alpha et gamma */
    alpha = y->left->left;
    gamma = y->right;

    /* Deplacement de beta a la place de alpha */
    if (y->left->right)
        y->left->left = y->left->right;
    else
        y->left->left = NULL;

    /* Deplacement de gamma a l'ancienne place de beta */
    if (gamma) {
        y->left->right = gamma;
        gamma->parent = y->left;
    } else {
        y->left->right = NULL;
    }

    /* Deplacement de la branche de gauche a droite */
    y->right = y->left;

    /* Deplacement de alpha sur la branche de gauche */
    if (alpha) {
        y->left = alpha;
        alpha->parent = y;
    } else {
        y->left = NULL;
    }

    /* Echange de x et y */
    bstree_swap_nodes(&y, y, y->right);
}

void testrotateleft(BinarySearchTree *t) {
    leftrotate(t);
}

void testrotateright(BinarySearchTree *t) {
    rightrotate(t);
}

ptrBinarySearchTree grandparent(ptrBinarySearchTree n) {
    if (n && n->parent)
        return n->parent->parent;
    else
        return NULL;
}

ptrBinarySearchTree uncle(ptrBinarySearchTree n) {
    ptrBinarySearchTree gp = grandparent(n);
    if (!gp) 
        return NULL;
    if (n->parent == gp->left)
        return gp->right;
    else
        return gp->left;
}

ptrBinarySearchTree fixredblack_insert(ptrBinarySearchTree x) {
    if (x->parent && x->parent->color == red)
        return fixredblack_insert_case0(x);
    else
        return x;
}

ptrBinarySearchTree fixredblack_insert_case0(ptrBinarySearchTree x) {
    ptrBinarySearchTree p = x->parent;
    if (p->parent == NULL) {
        p->color = black;
        return p;
    } else {
        return fixredblack_insert_case1(x);
    }
}

ptrBinarySearchTree fixredblack_insert_case1(ptrBinarySearchTree x) {
    ptrBinarySearchTree f = uncle(x);
    ptrBinarySearchTree pp = grandparent(x);
    if (f && f->color == red) {
        f->color = x->parent->color = black;
        pp->color = red;
        return fixredblack_insert(pp);
    } else {
        return fixredblack_insert_case2(x);
    }
}

ptrBinarySearchTree fixredblack_insert_case2(ptrBinarySearchTree x) {
    ptrBinarySearchTree pp = grandparent(x);
    if (x->parent == pp->left)
        return fixredblack_insert_case2_left(x);
    else
        return fixredblack_insert_case2_right(x); 
}

ptrBinarySearchTree fixredblack_insert_case2_left(ptrBinarySearchTree x) {
    ptrBinarySearchTree p = x->parent;
    ptrBinarySearchTree pp = grandparent(x);
    if (x == p->left) {
        rightrotate(pp);
        p->color = black;
        pp->color = red;
        return p; /* Nouvelle racine a cause de la rotation */
    } else {
        leftrotate(p);
        rightrotate(pp);
        x->color = black;
        pp->color = red;
        return x;
    }
}

ptrBinarySearchTree fixredblack_insert_case2_right(ptrBinarySearchTree x) {
    ptrBinarySearchTree p = x->parent;
    ptrBinarySearchTree pp = grandparent(x);
    if (x == p->right) {
        leftrotate(pp);
        p->color = black;
        pp->color = red;
        return p; /* Nouvelle racine a cause de la rotation */
    } else {
        rightrotate(p);
        leftrotate(pp);
        x->color = black;
        pp->color = red;
        return x;
    }
}
