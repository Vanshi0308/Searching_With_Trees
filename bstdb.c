#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.

// Struct to represent a book node in the Binary Search Tree
struct tree_book_node{
    char *name;
    int word_count;
    int book_id;
    struct tree_book_node *left_pointer;
    struct tree_book_node *right_pointer;
}tree_book_node;

int next_id;
struct tree_book_node *database;
int num_comps, num_searches, no_of_nodes, num_insertions;

static struct tree_book_node* create_new_node(char *name, int word_count, int id) {
    struct tree_book_node *new_node = (struct tree_book_node*)malloc(sizeof(struct tree_book_node));
    if(new_node) {
        new_node->word_count = word_count;
        new_node->book_id = id;
        new_node->left_pointer = NULL;
        new_node->right_pointer = NULL;
        int len_name = strlen(name)+1;
        new_node->name = calloc(len_name, sizeof(char));
        if (new_node->name) {
            strcpy( new_node->name, name );
        } 
        else {
            free(new_node);
            new_node = NULL;
        }
    }
    return new_node;
}

static struct tree_book_node* add_node_to_db(struct tree_book_node *root, int id, char *name, int word_count) {
    if(root==NULL) {
        root = create_new_node(name, word_count, id);
        return root;
    }
    else if(id < root->book_id) {
        root->left_pointer = add_node_to_db(root->left_pointer, id, name, word_count);
    }
    else {
        root->right_pointer = add_node_to_db(root->right_pointer, id, name, word_count);
    }

}

static struct tree_book_node* search_book(struct tree_book_node *root, int book_id) {
    if(root==NULL) {
        return NULL;
    }
    else if(root->book_id==book_id) {
        num_comps++;
        return root;
    }
    else if(root->book_id > book_id) {
        num_comps++;
        search_book(root->left_pointer, book_id);
    }
    else {
        num_comps++;
        search_book(root->right_pointer, book_id);
    }
}

static void find_no_of_nodes(struct tree_book_node *root) {
    if(root!=NULL) {
        no_of_nodes++;
    }
    if(root->left_pointer!=NULL) {
        find_no_of_nodes(root->left_pointer);
    }
    if(root->right_pointer!=NULL) {
        find_no_of_nodes(root->right_pointer);
    }
}

static void delete_tree(struct tree_book_node *root) {
    if(root==NULL) {
        return;
    }
    delete_tree(root->left_pointer);
    delete_tree(root->right_pointer);
    free(root);
}

int
bstdb_init ( void ) {
	// This function will run once (and only once) when the database first
	// starts. Use it to allocate any memory you want to use or initialize 
	// some globals if you need to. Function should return 1 if initialization
	// was successful and 0 if something went wrong.
    next_id = 0;
    database = NULL;
    num_comps = 0;
    num_searches = 0;
    no_of_nodes = 0;
    num_insertions = 0;
	return 1;
}

int
bstdb_add ( char *name, int word_count ) {
	// This function should create a new node in the binary search tree, 
	// populate it with the name and word_count of the arguments and store
	// the result in the tree.
	//
	// This function should also generate and return an identifier that is
	// unique to this document. A user can find the stored data by passing
	// this ID to one of the two search functions below.
	//
	// How you generate this ID is up to you, but it must be an integer. Note
	// that this ID should also form the keys of the nodes in your BST, so
	// try to generate them in a way that will result in a balanced tree.
	//
	// If something goes wrong and the data cannot be stored, this function
	// should return -1. Otherwise it should return the ID of the new node
    database = add_node_to_db(database, next_id, name, word_count);
    num_insertions++;
    next_id++;
	return (next_id-1);
}

int
bstdb_get_word_count ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1
    num_searches++;
    struct tree_book_node *p = search_book(database, doc_id);
	return (p)? p->word_count : -1;
}

char*
bstdb_get_name ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0
    num_searches++;
    struct tree_book_node *p = search_book(database, doc_id);
	return (p)? p->name : NULL;
}

void
bstdb_stat ( void ) {
	// Use this function to show off! It will be called once after the 
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//
	//  + Check if your tree is balanced and print the result
	//
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result? 
	//
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?
    printf("Checking stat function\n");
    printf("Average number of nodes traveresed to find a search result: %lf \n", (double)num_comps/num_searches);
    find_no_of_nodes(database);
    printf("No. of insertions: %i No. of nodes in tree: %i \n", num_insertions, no_of_nodes);
}

void
bstdb_quit ( void ) {
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.
    delete_tree(database);
}
