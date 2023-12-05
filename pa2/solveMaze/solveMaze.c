#include "../graphutils.h" // header for functions to load and free adjacencyList
#include "../queue/queue.h" // header for queue

// A program to solve a maze that may contain cycles using BFS

int main ( int argc, char* argv[] ) {

    // First, read the query file to get the source and target nodes in the maze
    /* ... */
    FILE* fp = fopen(argv[2], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    graphNode_t source;
    fscanf(fp, "%ld", &source);
    graphNode_t target;
    fscanf(fp, "%ld", &target);

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList = NULL;
    /* ... */
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);

    // USE A QUEUE TO PERFORM BFS
    Queue queue = { .front=NULL, .back=NULL };

    // An array that keeps track of who is the parent node of each graph node we visit
    graphNode_t* parents = calloc( graphNodeCount, sizeof(graphNode_t) );
    for (size_t i=0; i<graphNodeCount; i++) {
        parents[i] = -1; // -1 indicates that a nodes is not yet visited
    }

    /* ... */
    enqueue(&queue, &source);
    bool targetFound = false;

    // so long as we haven't found the target node yet, iterate through the adjacency list
    // add each neighbor that has not been visited yet (has no parents) to the queue of nodes to visit
    /* ... */
    //how to equate currentNodeName to index of parents? graphNode to size_t
    for (size_t b = 0; b < graphNodeCount; b++) {
        graphNode_t* current = (graphNode_t*) dequeue(&queue); // set current to first node
        AdjacencyListNode* adjacent = adjacencyList[*current].next; // look at neighbors of source
        while (adjacent != NULL) {
            if (parents[adjacent->graphNode] == -1) {
                parents[adjacent->graphNode] = *current;
                if (adjacent->graphNode == target) {
                    targetFound = true;
                    break;
                }
                enqueue(&queue, &(adjacent->graphNode));
            }
            adjacent = adjacent->next;
        }
        if (targetFound) {
            break;
        }
    }

    // Now that we've found the target graph node, use the parent array to print maze solution
    // Print the sequence of edges that takes us from the source to the target node
    /* ... */
    for (size_t k = 0; k < graphNodeCount; k++) {
        if (parents[k] != -1) {
            printf("%zu %zu\n", parents[k], k);
        }
    }

    // free any queued graph nodes that we never visited because we already solved the maze
    while ( queue.front ) {
        /* ... */
        dequeue(&queue);
    }
    free (parents);
    freeAdjList ( graphNodeCount, adjacencyList );

    return EXIT_SUCCESS;
}
