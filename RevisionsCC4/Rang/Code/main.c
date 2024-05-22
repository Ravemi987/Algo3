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
 * A node must defined its shape and its links to the left and right subtrees. If one of this subtree is NULL,
 * a blue box with the label NIL is drawn.
 * @dot
 * digraph node_example {
 *      node [shape = record];
 *      parent [label="tree_parent()"]
 *      treeroot [label="{{<parent>}|root|{<left>|<right>}}"];
 *      left [label="tree_left()"];
 *      right [label="tree_right()"];
 *      parent:s ->treeroot:parent:c [headclip=false, tailclip=false]
 *      treeroot:left:c -> left:n [headclip=false, tailclip=false]
 *      treeroot:right:c -> right:n [headclip=false, tailclip=false]
 * }
 * @enddot
 * @param t the tree node to draw.
 * @param userData the file to output the dot commands. Concretely, this parameter will be of type FILE * when the functor is called.
 */

void node_to_dot(const BinarySearchTree *t, void *userData) {
    FILE *file = (FILE *) userData;

    printf("%d ", tree_key(t));

    fprintf(file, "\tn%p [label=\"{%d|{<left>%d|<right>%d}}\"];\n",
            (void*)(t), tree_key(t), tree_lrank(t), tree_grank(t));

    if (tree_left(t)) {
        fprintf(file, "\tn%p:left:s -> n%p:n [headclip=false, tailclip=false]\n",
                (void*)(t), (void*)(tree_left(t)));
    } else {
        fprintf(file, "\tlnil%p [style=filled, fillcolor=dodgerblue, label=\"NIL\"];\n", (void*)(t));
        fprintf(file, "\tn%p:left:s -> lnil%p:n [headclip=false, tailclip=false, color=dodgerblue]\n",
                (void*)(t), (void*)(t));
    }

    if (tree_right(t)) {
        fprintf(file, "\tn%p:right:s -> n%p:n [headclip=false, tailclip=false]\n",
                (void*)(t), (void*)(tree_right(t)));
    } else {
        fprintf(file, "\trnil%p [style=filled, fillcolor=dodgerblue, label=\"NIL\"];\n", (void*)(t));
        fprintf(file, "\tn%p:right:s -> rnil%p:n [headclip=false, tailclip=false, color=dodgerblue]\n",
                (void*)(t), (void*)(t));
    }
}

/**
 * Print the value of a node.
 * @param t the tree node to output
 * @param userData unused pointer.
 */
void print_tree(const BinarySearchTree *t, void *userData) {
    (void) userData;
    printf("%d ", tree_key(t));
}

void exportTree(const BinarySearchTree *theTree, const char* filename) {
    char dotname[strlen(filename)+1];
    strcpy(dotname, filename);
    strcpy(dotname+strlen(dotname)-3, "dot");
    printf("Exporting the tree to %s.\n\t", dotname);
    FILE *output = fopen(dotname, "w");
    fprintf(output, "digraph ThreadedBinaryTree {\n\tgraph [ranksep=0.75, nodesep = 0.75];\n\tnode [shape = record];\n\n");
    tree_depth_prefix(theTree, node_to_dot, output);
    fprintf(output, "\n}\n");
    fclose(output);
    printf("\n");
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
int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "usage : %s filename\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror(argv[1]);
        return 1;
    }

    BinarySearchTree *theTree = tree_create();

    printf("Adding values to the tree.\n\t");
    int n = readInt(input);

    for (int i = 0; i < n; ++i) {
        int v = readInt(input);
        printf("%d, ", v);
        tree_add(&theTree, v);
    }
    printf("\b \b\b \b\n");

    exportTree(theTree, argv[1]);

    printf("Inorder visit of the tree : ");
    tree_inorder(theTree, print_tree, NULL);
    printf("\n");
    printf("Reverse order visit of the tree : ");
    tree_reverseorder(theTree, print_tree, NULL);
    printf("\n");

    printf("Tree has %d nodes.\n", tree_size(theTree));

    int nbp = readInt(input);
    printf("Searching K-th lower key of the tree.\n");
    for(int i=0; i< nbp; ++i) {
        int pg = readInt(input);
        printf("\t%d%s lower : %d\n", pg, pg == 1 ? "st" : (pg == 2 ? "nd" : (pg == 3 ? "rd" : "th")), tree_kth_lower(theTree, pg));
    } 

    int nbg = readInt(input);
    printf("Searching K-th greater key of the tree.\n");
    for(int i=0; i< nbg; ++i) {
        int pg = readInt(input);
        printf("\t%d%s greater : %d\n", pg, pg == 1 ? "st" : (pg == 2 ? "nd" : (pg == 3 ? "rd" : "th")), tree_kth_greater(theTree, pg));
    }

    fclose(input);
    return 0;
}
