#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char source[32];
char target[32];
int matrix[32][32];

size_t min ( size_t x, size_t y ) {
    return x<y ? x : y;
}

size_t levenshtein_recursive ( int m, int n) {

     //printf( "source=%s\n", source );
     //printf( "target=%s\n", target );
     //printf( "strlen=%d\n", i);
     //printf( "strlen=%d\n", j);

    //see for help: https://medium.com/@ethannam/understanding-the-levenshtein-distance-equation-for-beginners-c4285a5604f0

    if (m == 0) {
        return n;
    } else if (n == 0) {
        return m;
    } else if (matrix[m][n] != -1) { // ends
        return matrix[m][n];
    } else if (source[m - 1] == target[n - 1]) { // if same, move to the next and don't add 1
        return matrix[m][n] = levenshtein_recursive (m-1, n-1);
    } else { 
        size_t ins = levenshtein_recursive (m-1, n  );
        size_t del = levenshtein_recursive (m,   n-1);
        size_t sub = levenshtein_recursive (m-1, n-1);
        return matrix[m][n] = 1 + min( min(ins,del), sub );
    }

}

int main(int argc, char* argv[])
{

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    fscanf (inputFile, "%s\n%s", source, target);
    
    memset(matrix, -1, sizeof(matrix));
    printf("%ld\n", levenshtein_recursive ( strlen(source), strlen(target) ));

    return EXIT_SUCCESS;

}