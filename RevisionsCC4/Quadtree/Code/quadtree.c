#include "quadtree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------  Point  -----------------------------*/

Point create_point(float x, float y) {
    Point p = {x, y};
    return p;
}

/*------------------  Private representation of TAD Node  -------------------*/


/** opaque definition of type Node */
struct _treeNode {
    bool isleaf;
    Point min;
    Point max;
    union {
        struct {
            Point split;
            Node* upleft;
            Node* upright;
            Node* downleft;
            Node* downright;
        } childs;
        struct {
            Point* points;
            int nbpoints;
        } pointset;
    } data;
    Node* parent;
};

/*------------------  Public interface of TAD Node  -----------------------*/
/** Get the stored values on a leaf.
 * \@pre node_isleaf(n) == true;
 */
const Point* const node_values(const Node* const n, int* nbvalues) {
    assert(node_isleaf(n));
    *nbvalues = n->data.pointset.nbpoints;
    return n->data.pointset.points;
}

/** Accessors to the 4 sons of the node n.
 * @pre node_isleaf(n) == false;
 * @param n
 * @return
 */
Node* const node_upleft(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.upleft;
}

Node* const node_upright(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.upright;
}

Node* const node_downleft(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.downleft;
}

Node* const node_downright(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.downright;
}

Point node_splitpos(const Node* const n) {
    return n->data.childs.split;
}
/**
 * Is Node n a leaf ?
 * @param n
 * @return
 */
bool node_isleaf(const Node* const n) {
    return n->isleaf;
}


Node* node_create(Point min, Point max) {
    Node* n = malloc(sizeof(struct _treeNode));
    n->isleaf = true;
    n->min = min;
    n->max = max;
    n->data.pointset.nbpoints = 0;
    n->parent=NULL;
    return n;
}

int node_numpoints(const Node* n) {
    assert(node_isleaf(n));
    return n->data.pointset.nbpoints;
}

void node_add_point(Node* n, Point p) {
    assert(node_isleaf(n));
    ++(n->data.pointset.nbpoints);
    n->data.pointset.points = realloc(n->data.pointset.points, n->data.pointset.nbpoints* sizeof(Point));
    n->data.pointset.points[n->data.pointset.nbpoints-1] = p;
}

/*------------------  Private interface of TAD Node  -----------------------*/
void node_subdivide(Node* n);


/******************************************************************************/
/**                          Control start here                              **/
/******************************************************************************/
/**
 *  Nom         : Airiau
 *  Prenom      : Rémi
 *  Num Etud    : 22204545
 **/

/* Exercice 1 */
struct _quadtree {
    int capacity;
    Node *root;
};

QuadTree* quadtree_create(int npoints, Point min, Point max) {
    QuadTree *t = malloc(sizeof(struct _quadtree));
    t->capacity = npoints;
    t->root = node_create(min, max);
    return t;
}

bool quadtree_empty(const QuadTree* t) {
    if (node_isleaf(t->root)) {
        return node_numpoints(t->root) == 0;
    } else {
        return node_upleft(t->root) == NULL && node_upright(t->root) == NULL &&
            node_downleft(t->root) == NULL && node_downright(t->root) == NULL;
    }
}


/* Exercice 2 et Exercice 4
 */
void quadtree_add(QuadTree* t, Point p) {
    /* Exercice 2 */
    Node *current = t->root;
    while (!node_isleaf(current)) {
        Point s = node_splitpos(current);
        if (p.x < s.x && p.y < s.y)
            current = node_downleft(current);
        else if (p.x < s.x && p.y > s.y)
            current = node_upleft(current);
        else if (p.x > s.x && p.y < s.y)
            current = node_downright(current);
        else
            current = node_upright(current);
    }
    /* Exercice 4 */
    if (node_numpoints(current) < t->capacity) {
        node_add_point(current, p);
    } else {
        node_subdivide(current);
        quadtree_add(t, p);
    }
    /* Exercice 2 */
}

/* Exercice 3 */
void node_depth_prefix(const Node* n, OperateFunctor f, void* userData) {
    if (n == NULL) {
        return;
    } else {
        f(n, userData);
        if (!node_isleaf(n)) {
            node_depth_prefix(node_upleft(n), f, userData);
            node_depth_prefix(node_upright(n), f, userData);
            node_depth_prefix(node_downleft(n), f, userData);
            node_depth_prefix(node_downright(n), f, userData);
        }
    }
}

/** Depth first, prefix visitor */
void quadtree_depth_prefix(const QuadTree* t, OperateFunctor f, void* userData){
    if (quadtree_empty(t)) {
        return;
    } else {
        node_depth_prefix(t->root, f, userData);
    }
}


/* Exercice 4 */
void node_subdivide(Node* n) {
    assert(node_isleaf(n));

    float splitpos_x = (n->min.x + n->max.x) / 2;
    float splitpos_y = (n->min.y + n->max.y) / 2;

    Node* upleft = node_create(create_point(n->min.x, splitpos_y), create_point(splitpos_x, n->max.y));
    Node* upright = node_create(create_point(splitpos_x, splitpos_y), create_point(n->max.x, n->max.y));
    Node* downleft = node_create(create_point(n->min.x, n->min.y), create_point(splitpos_x, splitpos_y));
    Node* downright = node_create(create_point(splitpos_x, n->min.y), create_point(n->max.x, splitpos_y));

    int nbValues;
    const Point* tab = node_values(n, &nbValues);

    for (int i = 0; i < nbValues; i++) {
        Point p = tab[i];
        if (p.x < splitpos_x && p.y < splitpos_y) {
            node_add_point(downleft, p);
        } else if (p.x < splitpos_x && p.y > splitpos_y) {
            node_add_point(upleft, p);
        } else if (p.x >= splitpos_x && p.y <= splitpos_y) {
            node_add_point(downright, p);
        } else {
            node_add_point(upright, p);
        }
    }

    free(n->data.pointset.points);

    n->isleaf = false;
    n->data.childs.split.x = splitpos_x;
    n->data.childs.split.y = splitpos_y;
    n->data.childs.upleft = upleft;
    n->data.childs.upright = upright;
    n->data.childs.downleft = downleft;
    n->data.childs.downright = downright;

    upleft->parent = n;
    upright->parent = n;
    downleft->parent = n;
    downright->parent = n;
}
