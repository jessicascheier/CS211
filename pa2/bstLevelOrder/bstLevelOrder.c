#include <stdlib.h>
#include <stdio.h>
#include "../bstReverseOrder/bst.h"
#include "../queue/queue.h"

// A program to perform a LEVEL ORDER (BREADTH-FIRST) TRAVERSAL of a binary search tree
int main ( int argc, char* argv[] ) {

    // READ INPUT FILE TO CREATE BINARY SEARCH TREE
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    BSTNode* root = NULL;
    int key;
    while ( fscanf(fp, "%d", &key)!=EOF ) {
        root = insert (root, key);
    }
    fclose(fp);

    // USE A QUEUE TO PERFORM LEVEL ORDER TRAVERSAL
    Queue queue = { .front=NULL, .back=NULL };
    BSTNode* temp_node = root;

    while (temp_node) {
        printf("%d ", temp_node->key);
 
        /*Enqueue left child */
        if (temp_node->l_child)
            enqueue(&queue, temp_node->l_child);
 
        /*Enqueue right child */
        if (temp_node->r_child)
            enqueue(&queue, temp_node->r_child);
 
        /*Dequeue node and make it temp_node*/
        temp_node = dequeue(&queue);
    }
    /* ... */

    delete_bst(root);
    return EXIT_SUCCESS;
}
