#include <stdio.h>
#include <stdlib.h>
#include "tree_utils.h"

#define MARKER -1

Node* new_node(int ch) {
    // Allocate memory for new node
    Node* node = (Node*)malloc(sizeof(Node));

    // Assign data to this node
    node->character = ch;

    // Initialize left and right children as NULL
    node->left = NULL;
    node->right = NULL;
    return(node);
}

// This function stores a tree in a file pointed by fp
void serialize(Node* node, FILE* fp) {
    // If current node is NULL, store marker
    if (node == NULL) {
        fprintf(fp, "%d ", MARKER);
        return;
    }

    // Else, store current node and recur for its children
    fprintf(fp, "%d ", node->character);
    serialize(node->left, fp);
    serialize(node->right, fp);
}

// This function constructs a tree from a file pointed by 'fp'
void deserialize(Node** node, FILE* fp) {
    // Read next item from file. If theere are no more items or next
    // item is marker, then return
    int ch;
    if (!fscanf(fp, "%d ", &ch) || ch == MARKER)
        return;

    // Else create node with this item and recur for children
    *node = new_node(ch);
    deserialize(&(*node)->left, fp);
    deserialize(&(*node)->right, fp);
}

void place_in_tree(Node* node, int ch, char* code) {
    if (*code == '\0')
        node->character = ch;
    else {
        if (*code == '.') {
            if (node->left == NULL)
                node->left = new_node('\0');
            place_in_tree(node->left, ch, ++code);
        } else if (*code == '-') {
            if (node->right == NULL)
                node->right = new_node('\0');
            place_in_tree(node->right, ch, ++code);
        } else
            return;
    }
}

void print_traversal(Node* node) {
    if (node == NULL) return;
    if (node->character != '\0') {
        putchar(node->character);
        putchar('\n');
    } else {
        printf("(null)");
        putchar('\n');
    }
    print_traversal(node->left);
    print_traversal(node->right);
}
