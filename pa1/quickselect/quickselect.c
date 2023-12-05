#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// A translation from pseudocode to C code
// https://en.wikipedia.org/wiki/Quicksort
// Hoare's partition scheme
// Divides array into two partitions
int partition ( int array[], int left, int right ) {

    // Pivot value
    int pivot = array[ right ]; // The value in the middle of the array

    // Left index
    int i = left - 1;

    // Right index
    int j = left;

    for (j = left; j < right; j++) {
        if (array[j] < pivot) {
            i++; //i and j loops to keep track of items to switch
            int temp = array[i]; // swap
            array[i] = array[j];
            array[j] = temp;
        }
    }
    int temp = array[i + 1]; // swap
    array[i + 1] = array[right];
    array[right] = temp;
    return (i + 1);
}

int quickselect(int array[], int left, int right, int k) {
    if (left == right) return array[left];
    int pivot_index = partition( array, left, right );
    if (pivot_index == k) {
        return array[pivot_index];
    }
    if (k < pivot_index) {
        return quickselect(array, left, pivot_index - 1, k);
    }
    return quickselect(array, pivot_index + 1, right, k);
}

int main(int argc, char* argv[])
{

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    char buf[256];

    char* len_string = fgets(buf, 256, inputFile);
    int len = atoi(len_string);
    char* kth_string = fgets(buf, 256, inputFile);
    int kth = atoi(kth_string);
    fgets(buf, 256, inputFile);

    int* array = calloc( len, sizeof(int) );

    for (int i=0; i<len; i++) {
        char* int_string = fgets(buf, 256, inputFile);
        array[i] = atoi(int_string);
    }

    int left = 0;
    int right = len - 1;
    int kth_smallest_element = quickselect(array, left, right, kth);
    printf("%d\n", kth_smallest_element);

    free(array);

    return EXIT_SUCCESS;

}
