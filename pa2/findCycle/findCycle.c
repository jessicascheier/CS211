#include "../graphutils.h"

// A program to find a cycle in a directed graph

// You may use DFS or BFS as needed
bool isCyclic = false;

void dfs(graphNode_t current, bool* visited, bool* inpath, AdjacencyListNode* adjacencyList, size_t graphNodeCount, graphNode_t* stack, graphNode_t count) {

    visited[current] = true;
    inpath[current] = true; // mark source node as in the path
    stack[count] = current; // count corresponds to when node was added, increments whenever dfs is run

    for (AdjacencyListNode* adjacent = adjacencyList[current].next; adjacent != NULL; adjacent = adjacent->next) { // loop through its neighbors
        
        graphNode_t adjacentNodeName = adjacent->graphNode; // turn linkedlist node into graphNode

        if (inpath[adjacentNodeName]) { // if it's already in the path, check if it's in the stack

            for (graphNode_t m = 0; m <= count; m++) { // loop through stack

                if (stack[m] == adjacentNodeName) { // if duplicate in the stack, cycle is true

                    isCyclic = true; // cycle is found

                    for (graphNode_t n = m; n <= count; n++) { // print dupe and everything after to include entire cycle
                        
                        if (stack[n] != -1) { // since stack was initialized with all -1s, make sure you're not printing those accidentally
                            printf("%ld ", stack[n]);
                        }

                    }
                    printf("\n");
                    exit(EXIT_SUCCESS); // used to exit program, return or break will not work, had to look this up
                }
                
            }

        } else if (!visited[adjacentNodeName]) {

            if (!isCyclic) {
                dfs(adjacentNodeName, visited, inpath, adjacencyList, graphNodeCount, stack, count + 1);
                // keep running until you hit a cycle
            }

        }
    }

    inpath[current] = false; // not visited anymore once going to the next starting node from main
    return;
}

int main ( int argc, char* argv[] ) {

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList;
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);

    for (graphNode_t source=0; source<graphNodeCount; source++) {

        graphNode_t stack[graphNodeCount];
        for (graphNode_t m = 0; m < graphNodeCount; m++) {
            stack[m] = -1; // initialize
        }
 
        bool inpath[graphNodeCount];
        bool visited[graphNodeCount];
        
        for (size_t i = 0; i < graphNodeCount; i++) {
            inpath[i] = false;
            visited[i] = false;
        }

        graphNode_t count = 0;
        
        if (!(inpath[source])) {
            dfs(source, visited, inpath, adjacencyList, graphNodeCount, stack, count);
        }

        if (isCyclic) {
            break;
        }
    }

    if (!isCyclic) { printf("DAG\n"); }

    freeAdjList ( graphNodeCount, adjacencyList );
    return EXIT_SUCCESS;
}
