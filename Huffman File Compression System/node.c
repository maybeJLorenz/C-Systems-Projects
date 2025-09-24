/*
 *
 *	node.c file
 *
 */

#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ( done ) - have it checked
Node *node_create(uint8_t symbol, uint32_t weight) {

    // create a <Node> and set its <symbol> and <weight> fields. Return
    // a pointer to the new node. On error, return NULL.

    Node *n = (Node *) calloc(1, sizeof(Node));

    if (!n) {
        return NULL; // allocation failed
    }

    // setting values
    n->symbol = symbol;
    n->weight = weight;

    /*  

    if (n->weight == 0) {
        free(n);
        return NULL; // failed to set values
    }
*/

    return n;
}

// ( done ) - ready to be checked
void node_free(Node **node) {

    if (*node != NULL) {

        node_free(&(*node)->left);
        node_free(&(*node)->right);

        free(*node);
        *node = NULL;
    }

    return;
}

// ( done ) - given, but it's not in node.h
void node_print_node(Node *tree, char ch, int indent) {

    if (tree == NULL) {
        return;
    }

    node_print_node(tree->right, '/', indent + 3);
    printf("%*cweight = %d", indent + 1, ch, tree->weight);

    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }

    printf("\n");
    node_print_node(tree->left, '\\', indent + 3);
}

void node_print_tree(Node *tree) {

    node_print_node(tree, '<', 2);
}
