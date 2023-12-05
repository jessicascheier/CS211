#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) {

  // Open the filename given as the first command line argument for reading
  FILE* fp = fopen(argv[1], "r");
  if (!fp) {
    perror("fopen failed");
    return EXIT_FAILURE;
  }

  char buf[256];

  char* string = fgets(buf, 256, fp);
  int x = atoi(string);
  char* string2 = fgets(buf, 256, fp);
  int y = atoi(string2);

  int tmp = sqrt((x * x) + (y * y));
  int tmp2 = sqrt((x * x) - (y * y));
  int tmp3 = sqrt((y * y) - (x * x));
  if (tmp * tmp == (x * x) + (y * y)) {
    printf("%d\n", tmp);
  } else if (tmp2 * tmp2 == (x * x) - (y * y)) {
    printf("%d\n", tmp2);
  } else if (tmp3 * tmp3 == (y * y) - (x * x)) {
    printf("%d\n", tmp3);
  } else {
    printf("%d\n", -1);
  }
  
  // Printing in C.
  // %d is the format specifier for integer numbers.
  // \n is the newline character
  /*printf( "%d\n", x );*/
  return EXIT_SUCCESS;

}