#include <stdio.h>
#include <stdlib.h>
#include "bst.h"


void tree_insert(Tree_Node** root, char data){
    if(*root==NULL) {
        struct Tree_Node *newNode = (struct Tree_Node*)malloc(sizeof(struct Tree_Node));
        newNode->data = data;
        newNode->left_pointer = NULL;
        newNode->right_pointer = NULL;
        *root = newNode;
    }
    else if(data <= (*root)->data) {
        tree_insert(&((*root)->left_pointer), data);
    }
    else {
        tree_insert(&((*root)->right_pointer), data);
    }
}

Tree_Node* create_bst (char data[]){
    struct Tree_Node *root = NULL; 
    int i = 0;
    while(data[i]!='\0') {
        tree_insert(&root, data[i]);
        i++;
    }
    return root;
}

Tree_Node* tree_search(Tree_Node* root, char data){
    if(root==NULL) {
        return NULL;
    }
    else if(root->data==data) {
        return root;
    }
    else if(root->data > data){
        tree_search(root->left_pointer, data);
    }
    else {
        tree_search(root->right_pointer, data);
    }
}

void tree_print_sorted(Tree_Node* root){
    if (root == NULL) {
        // printf("--<empty>--\n");
        return;
    }
    else {
        tree_print_sorted(root->left_pointer);
        printf("%c", root->data);
        tree_print_sorted(root->right_pointer);
    }
}

void tree_delete(Tree_Node* root){
    if(root==NULL) {
        return;
    }
    tree_delete(root->left_pointer);
    tree_delete(root->right_pointer);
    free(root);
}

