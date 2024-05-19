#include "trie.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include <stdio.h>

/* -------------------------------------------- */
/*	Node declaration							*/
/* -------------------------------------------- */

typedef struct s_Node Node;

/**
 * Creates a node with its terminal status defined by b
 */
Node* nd_create( bool b );

/**
 * Free the memory used by the node *n
 */
void nd_delete( Node** n );

/**
 * Add the node c as a child of the node p for the link l
 */
void nd_addChild( Node* p, char l, Node* c );

/**
 * Mark the node as terminal
 */
void nd_setTerminal( Node* n );

/**
 * Returns true if the node is terminal, false if not
 */
bool nd_isTerminal( const Node* n );

/**
 * Returns true if the node n has a child for the link l, false if not
 */
bool nd_hasChild( const Node* n, char l );

/**
 * Returns the child of the node n for the link l
 */
Node* nd_getChild( const Node* n, char k );

/* -------------------------------------------- */
/*	Node definition  							*/
/* -------------------------------------------- */

struct s_Node {
    Node* links[26]; // array of child nodes
    bool terminal;   // end of word marker
};

Node* nd_create( bool b ) {
    Node* n = malloc( sizeof( struct s_Node ) );
    memset( n->links, 0, 26 * sizeof( Node* ) );
    n->terminal = b;
    return n;
}

void nd_delete( Node** n ) {
    free( *n );
    *n = NULL;
}

void nd_addChild( Node* p, char l, Node* c ) {
    int index = l - 'A';
    assert( 0 <= index && index < 26 );
    p->links[index] = c;
}

void nd_setTerminal( Node* n ) {
    n->terminal = true;
}

bool nd_isTerminal( const Node* n ) {
    return n->terminal;
}

bool nd_hasChild( const Node* n, char l ) {
    int index = l - 'A';
    assert( 0 <= index && index < 26 );
    assert( n != NULL );
    return n->links[index] != NULL;
}

Node* nd_getChild( const Node* n, char k ) {
    int index = k - 'A';
    assert( 0 <= index && index < 26 );
    return n->links[index];
}

/* -------------------------------------------- */
/*	Trie definition								*/
/* -------------------------------------------- */

/**
 * Creates a trie whose root is associated to an empty key
 */
Trie* trie_create() {
    Trie* t = nd_create( true );
    return t;
}

/**
 * Free the memory used by the trie *t. After deletion, *t is NULL
 */
void trie_delete( Trie** t ) {
    Trie* a = *t;
    Node* s;
    for ( char c = 'A'; c <= 'Z'; ++c ) {
        if ( ( s = nd_getChild( a, c ) ) != NULL ) { trie_delete( &s ); }
    }
    nd_delete( t );
}

/**
 * Test if the root node of the Trie t is a terminal node is terminal
 */
bool trie_isRootTerminal( const Trie* t ) {
    return nd_isTerminal( t );
}

/**
 * Test if the the Trie t has a child for the link l
 */
bool trie_hasChild( const Trie* t, char l ) {
    return nd_hasChild( t, l );
}

/**
 * Get the child of the trie t for the link l
 */
Trie* trie_getChild( const Trie* t, char l ) {
    return nd_getChild( t, l );
}

/***************************************************************************************/
/**                                 Control start here                                **/
/***************************************************************************************/
/**
 *  Nom : Airiau                     Prenom : Rémi                   Num Etud  : 22204545
 **/
/*
 *  Barème :
 *    - Compilation sans warnings : 2 pts
 *    - Exercice 1 : 4 pts
 *    - Exercice 2 : 3 pts
 *    - Exercice 3 : 4 pts
 *    - Exercice 4 : 5 pts
 *    - 2 points supplémentaires selon la qualité, clarté et concision du code.
 */

/***************************************************************************************/
// Exercice 1.1 : trie_add (iterative)
void trie_add( Trie* t, const char* word ) {
    char *c = (char*)word;
    Node *current = t;
    int i = 0;
    while (c[i] != '\0') {
        if (nd_hasChild(current, c[i])) {
            current = nd_getChild(current, c[i]);
        } else {
            Node *n = nd_create(false);
            nd_addChild(current, c[i], n);
            current = n;
        }
        i++;
    }
    nd_setTerminal(current);
}

// Exercice 1.2 trie_map (recursive, call functor on all nodes)
void trie_map_rec(const Trie* t, TrieMapOperator f, void* userdata) {
    Trie *current = (Trie*)t;
    Node *n;
    for (char c = 'A'; c <= 'Z'; ++c) {
        if ((n = nd_getChild(current, c)) != NULL) {
            f(n, c, userdata);
            trie_map_rec(n, f, userdata);
        }
    }
}

void trie_map( const Trie* t, TrieMapOperator f, void* userdata ) {
    f(t, '\0', userdata);
    trie_map_rec(t, f, userdata);
}

/***************************************************************************************/
// Exercice 2 : trie_search (iterative, constant storage)
bool trie_search( const Trie* t, const char* word ) {
    Node *current = (Trie*)t;
    int i = 0;
    while (word[i] != '\0') {
        if (nd_hasChild(current, word[i])) {
            current = nd_getChild(current, word[i]);
        } else {
            return false;
        }
        i++;
    }
    return nd_isTerminal(current);
}

/***************************************************************************************/
// exercice 3 trie_visit_helper (recursive, call functor only on terminal nodes)

void trie_visit_helper(const Trie* t, const char* prefix, TrieVisitFunctor f, void* userdata) {
    size_t lp = strlen(prefix) ;
    char newprefix[lp+2];
    strcpy(newprefix, prefix) ;
    newprefix[lp +1] = '\0';
    Trie *current = (Trie*)t;
    Node *n;
    for (char c = 'A'; c <= 'Z'; ++c) {
        if ((n = nd_getChild(current, c)) != NULL) {
            newprefix[lp] = c;
            if (trie_isRootTerminal(n))
                f(newprefix, userdata);
            trie_visit_helper(n, newprefix, f, userdata);
        }
    }
}

void trie_visit( const Trie* t, TrieVisitFunctor f, void* userdata ) {
    if (trie_isRootTerminal(t))
        f("", userdata);
    trie_visit_helper(t, "", f, userdata);
}

/***************************************************************************************/
// Exercice 4 : completion
// Exercice 4.1 : trie_suffix (iterative)
const Trie* trie_suffix( const Trie* t, const char* p) {
    Node *current = (Trie*)t;
    int i = 0;
    while (p[i] != '\0') {
        if (nd_hasChild(current, p[i])) {
            current = nd_getChild(current, p[i]);
        } else {
            return NULL;
        }
        i++;
    }
    return current;
}

// Exercice 4.2 :  trie_get_completion (using trie-prefix and trie_visit to accumulate words on a
// string array) structure for accumulating words

void sa_add_functor(const char* word, void* userdata) {
    sa_add((StringArray*)userdata, word);
    
}

void add_prefix(const char* word, void* userdata) {
    char* suffix = malloc(sizeof(char)*(strlen(word) + 1));
    char* prefix = malloc(sizeof(char)*(strlen((char*)userdata) + 1));
    strcpy(suffix, (char*)word);
    strcpy(prefix, (char*)userdata);
    strcat(prefix, suffix);
    strcpy((char*)word, prefix);
    free(prefix);
    free(suffix);
}

StringArray* trie_get_completion( const Trie* t, const char* p ) {
    StringArray *a = sa_create();
    Trie *r = (Trie*)trie_suffix(t, p);
    if (r) trie_visit(r, sa_add_functor, a);
    sa_visit(a, add_prefix, (void*)p);
    return a;
}
