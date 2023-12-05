#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Markers used to bound trace regions of interest */
volatile char MARKER_START, MARKER_END;

int main(int argc, char* argv[])
{

    /* Record marker addresses */
    FILE* marker_fp = fopen(".marker","w");
    assert(marker_fp);
    fprintf(marker_fp, "%llx\n%llx",
        (unsigned long long int) &MARKER_START,
        (unsigned long long int) &MARKER_END );
    fclose(marker_fp);

    if (argc!=3) {
        printf("Usage: ./cacheBlocking <matrix_a_file> <matrix_b_file>\n");
        exit(EXIT_FAILURE);
    }

    FILE* matrix_a_fp = fopen(argv[1], "r");
    if (!matrix_a_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    size_t n;
    int ret = fscanf(matrix_a_fp, "%ld\n", &n);
    assert (ret==1);
    int* a = calloc( n*n, sizeof(int) );
    for ( size_t i=0; i<n; i++ ) {
        for ( size_t k=0; k<n; k++ ) {
            ret = fscanf(matrix_a_fp, "%d ", &a[i*n+k]);
            assert (ret==1);
        }
        ret = fscanf(matrix_a_fp, "\n");
    }
    fclose(matrix_a_fp);

    FILE* matrix_b_fp = fopen(argv[2], "r");
    if (!matrix_b_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    size_t m;
    ret = fscanf(matrix_b_fp, "%ld\n", &m);
    assert (ret==1);
    assert( n==m );
    int* b = calloc( n*n, sizeof(int) );
    for ( size_t k=0; k<n; k++ ) {
        for ( size_t j=0; j<n; j++ ) {
            ret = fscanf(matrix_b_fp, "%d ", &b[k*n+j]);
            assert (ret==1);
        }
        ret = fscanf(matrix_b_fp, "\n");
    }
    fclose(matrix_b_fp);

    int* c = calloc( n*n, sizeof(int) );
    MARKER_START = 211;

    /*for (int row = 0; row < n; row+=blockSize) {
        for (int col = 0; col < n; col+=blockSize) {
            for (int blockRow = row; blockRow < row + blockSize; blockRow++) {
                for (int blockCol = col; blockCol < col + blockSize; blockCol++) {
                    int sum = 0;
                    for (int dot = 0; dot < n; dot++) {
                        sum += a[blockRow * n + dot] * b[dot * n + blockCol];
                    }
                    c[blockRow * n + blockCol] += sum;
                }
            }
        }
    }*/

    // see youtube video that helped: https://youtu.be/G92BCtfTwOE !!!!
    register int blockSize = 4; // register helps it being accessed faster
    for (size_t row = 0; row < n; row += blockSize) {
        for (size_t col = 0; col < n; col += blockSize) {
            for (size_t blockRow = 0; blockRow < n; blockRow++) { // similar to my last attempt, moved this loop outside of the other two loops
                for (size_t blockCol = row; blockCol < row + blockSize; blockCol++) {
                    register int a_val = a[(blockRow * n) + blockCol]; // for some reason this saves more time
                    for (size_t dot = col; dot < col + blockSize; dot++) { // compute dot product
                        c[(blockRow * n) + dot] += a_val * b[(blockCol * n) + dot];
                    }
                }
            }
        }
    }

    MARKER_END = 211;

    for ( size_t i=0; i<n; i++ ) {
        for ( size_t j=0; j<n; j++ ) {
            printf( "%d ", c[i*n+j] );
        }
        printf( "\n" );
    }

    free(c);
    free(b);
    free(a);
    exit(EXIT_SUCCESS);

}
