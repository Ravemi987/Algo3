#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


int readInt(FILE* input) {
  int v;
  int r = fscanf(input,"%d", &v);
  if (r == 1) {
    return v;
  }
  perror("Unable to read int from input file\n");
  abort();
}

/**
 * This function output one node using the <a href="https://www.graphviz.org/documentation/">dot</a> syntax.
 * A node must defined its shape and its links to the left and right subtrees. If one of this subtree does not exist,
 * a grey box with the label NIL is drawn.
 * @dot
 * digraph StaticSearchTree {
 * 	  graph [ranksep=0.75, nodesep = 0.75];
 * 	  node [shape = record];
 * 	  n0 [label="{{<parent>}|4|{<left>|<right>}}"];
 * 	  lnil0 [style=filled, fillcolor=dodgerblue, label="NIL"];
 * 	  n0:left:c -> lnil0:n [headclip=false, tailclip=false, color=dodgerblue]
 * 	  rnil0 [style=filled, fillcolor=dodgerblue, label="NIL"];
 * 	  n0:right:c -> rnil0:n [headclip=false, tailclip=false, color=dodgerblue]
 * }
 * @enddot
 * @param t the tree node to draw.
 * @param userData the file to output the dot commands. Concretely, this parameter will be of type FILE * when the functor is called.
 */

void node_to_dot(const StaticSearchTree *t, int node_id, void *userData) {
    FILE *file = (FILE *) userData;

    printf("%d ", tree_key(t, node_id));

    fprintf(file, "\tn%d [label=\"{{<parent>}|%d|{<left>|<right>}}\"];\n",
            node_id, tree_key(t, node_id));

    int child;
    child = tree_left(t, node_id);
    if ( tree_isvalidnode(t, child) ) {
        fprintf(file, "\tn%d:left:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                node_id, child);
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=dodgerblue, label=\"NIL\"];\n", node_id);
        fprintf(file, "\tn%d:left:c -> lnil%d:n [headclip=false, tailclip=false, color=dodgerblue]\n",
                node_id, node_id);
    }

    child = tree_right(t, node_id);
    if ( tree_isvalidnode(t, child) ) {
        fprintf(file, "\tn%d:right:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                node_id, child);
    } else {
        fprintf(file, "\trnil%d [style=filled, fillcolor=dodgerblue, label=\"NIL\"];\n", node_id);
        fprintf(file, "\tn%d:right:c -> rnil%d:n [headclip=false, tailclip=false, color=dodgerblue]\n",
                node_id, node_id);
    }
}

void exportTree(const StaticSearchTree *theTree, const char* filename) {
    char dotname[strlen(filename)+1];
    strcpy(dotname, filename);
    strcpy(dotname+strlen(dotname)-3, "dot");
    printf("Exporting the tree to %s : ", dotname);
    FILE *output = fopen(dotname, "w");
    fprintf(output, "digraph StaticSearchTree {\n\tgraph [ranksep=0.75, nodesep = 0.75];\n\tnode [shape = record];\n\n");
    tree_map_on_nodes(theTree, node_to_dot, output);
    fprintf(output, "\n}\n");
    fclose(output);
    printf("\n");
}

/**
 * Print the value of a node.
 * @param t the tree node to output
 * @param userData unused pointer.
 */
void print_tree(const StaticSearchTree *t, int nodeidx, void *userData) {
    (void) userData;
    printf("%d ", tree_key(t, nodeidx));
}

/** Main function for testing the Tree implementation.
 * The main function expects one parameter that is the file where values added to the tree, searched into the
 * tree and removed from the tree are to be read.
 *
 * This file must contain the following informations :
 * - on the first line, the number of values to be added to the tree,
 * - on the second line, the values to be added, separated by a space (or tab).
 *
 * The values will be added in the order they are read from the file.
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "usage : %s filename\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror(argv[1]);
        return 1;
    }

    StaticSearchTree *theTree = tree_create();

    printf("Adding keys to the tree : ");
    int n = readInt(input);

    for (int i = 0; i < n; ++i) {
        int v = readInt(input);
        int p = tree_add(theTree, v); // Exercice 1
        printf("%d (%d) - ", p, v);
        
    }
    printf("\b \b\b \b\n");

    exportTree(theTree, argv[1]); // Exercice 1

    int ns = readInt(input);
    if (ns > 0) {
        printf("Searching keys in the tree.\n"); // Exercice 2
        for(int i=0; i<ns; ++i) {
            int v = readInt(input);
            int id = tree_search(theTree, v);
            if (tree_isvalidnode(theTree, id) ) {
                printf("\tValue %d found at index %d.\n", v, id);
            } else {
                printf("\tValue %d not found.\n", v);
            }
        }
    }

    // Exercice 3
    printf("Inorder visit of the tree : ");
    tree_inorder(theTree, print_tree, NULL);
    printf("\n");

    printf("Reverse order visit of the tree : ");
    tree_reverseorder(theTree, print_tree, NULL);
    printf("\n");

    // Exercice 4
    int nc = readInt(input);
    if (nc > 0) {
        printf("Searching nearest common ancestor of keys.\n");
        for(int i=0; i<nc; ++i) {
            int k1 = readInt(input);
            int k2 = readInt(input);
            int lca = tree_nearest_common_ancestor(theTree, k1, k2);
            printf( "\tnearest_common_ancestor(%d, %d) --> %d (%d)\n", k1, k2, tree_key(theTree, lca), lca);
        }
    }
    
    // Exercice 5
    nc = readInt(input);
    if (nc > 0) {
        printf("Computing distances between keys.\n");
        for(int i=0; i<nc; ++i) {
            int k1 = readInt(input);
            int k2 = readInt(input);
            int d = tree_distance(theTree, k1, k2);
            printf( "\tdistance(%d, %d) --> %d \n", k1, k2, d);
        }
    }

    fclose(input);
    return 0;
}
