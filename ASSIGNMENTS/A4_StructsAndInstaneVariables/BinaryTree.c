#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    int value;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
};

struct Node* newNode(int val) {
    struct Node *my_node = (struct Node*) malloc(sizeof(struct Node));
    my_node->value = val;
    my_node->parent = NULL;
    my_node->left = NULL;
    my_node->right = NULL;
    return my_node;
}

void insert(struct Node* my_node, struct Node *n){
    if (n->value <= my_node->value) {
        if (my_node->left == NULL) {
            my_node->left = n;
            n->parent = my_node;
        } else {
            insert(my_node->left, n);
        }
    } else {
        if (my_node->right == NULL) {
            my_node->right = n;
            n->parent = my_node;
        } else {
            insert(my_node->right, n);
        }
    }
}

void printInOrder(struct Node* my_node) {
    if (my_node->left != NULL){
        printInOrder(my_node->left);
    }
    printf(" %d", my_node->value);
    if (my_node->right != NULL){
        printInOrder(my_node->right);
    }
}


void printPath(struct Node* my_node) {
    if (my_node->parent != NULL) {
        printPath(my_node->parent);
    }
    printf("%s: %d\n", my_node->parent == NULL? "from root": my_node->parent->left == my_node? "left to": "right to", my_node->value);
}


int main (int argc, char *argv[]) {
    struct Node *root = NULL;
    // read values from command line and add them to the tree
    struct Node *lastNodeInserted = NULL;

    for (int i = 1; i < argc; i++) {
        int value = atoi(argv[i]);
        struct Node* n = newNode(value);
        if (root == NULL){
            root = n;
        } else {
            insert(root, n);
        }
        lastNodeInserted = n;
    }

    // print results
    if (root != NULL) {
        printf("In Order:");
        printInOrder(root);
        printf("\nPath to %d:\n", lastNodeInserted->value);
        printPath(lastNodeInserted);
        free(lastNodeInserted);
    }
}