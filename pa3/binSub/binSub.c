#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
// https://www.tutorialspoint.com/c_standard_library/limits_h.htm
#include <limits.h>

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    //for (int i=CHAR_BIT-1; 0<=i; i--)
    // SETUP

    // first, read the minuend (number to be subtracted from)
    char buff;
    bool minuend[CHAR_BIT]; // suggested that you store bits as array of bools; minuend[0] is the LSB
    for (int i=CHAR_BIT-1; 0<=i; i--) { // read MSB first as that is what comes first in the file
        buff = fgetc(fp);
        if (buff == '\n') {
            break;
        } else if (buff == 49) {
            minuend[i] = true;
        } else {
            minuend[i] = false;
        }
    }
    /*for (int i=CHAR_BIT-1; 0<=i; i--) {
        printf("%d", minuend[i]);
    }
    printf("\n");*/

    // notice that you are reading two different lines; caution with reading
    /* ... */
    fgetc(fp);

    // second, read the subtrahend (number to subtract)
    bool subtrahend[CHAR_BIT]; // suggested that you store bits as array of bools; subtrahend[0] is the LSB
    char buffer;
    for (int i=CHAR_BIT-1; 0<=i; i--) { // read MSB first as that is what comes first in the file
        buffer = fgetc(fp);
        if (buffer == 49) {
            subtrahend[i] = true;
        } else {
            subtrahend[i] = false;
        }
    }

    // WE WILL DO SUBTRACTION BY NEGATING THE SUBTRAHEND AND ADD THAT TO THE MINUEND

    // Negate the subtrahend
    // flip all bits
    for (int i=CHAR_BIT-1; 0<=i; i--) {
        if (subtrahend[i] == 1) {
            subtrahend[i] = 0;
        } else {
            subtrahend[i] = 1;
        }
    }

    // add one
    bool carry = true; // to implement the 'add one' logic, we do binary addition logic with carry set to true at the beginning
    for (int i=0; i<CHAR_BIT; i++) { // iterate from LSB to MSB
        if (subtrahend[i] == 0 && carry) {
            subtrahend[i] = 1;
            carry = false;
        } else if (subtrahend[i] == 1 && carry) {
            carry = true;
            subtrahend[i] = 0;
        }
    }
    /*for (int i=CHAR_BIT-1; 0<=i; i--) {
        printf("%d", subtrahend[i]);
    }
    printf("\n");*/

    // first number plus second number

    // 0 + 0 = 0, if carry = true, make it 1
    // 1 + 0 = 1, if carry = true, carry again, make it 0
    // 0 + 1 = 1, if carry = true, carry again, make it 0
    // 1 + 1 = 10, carry = true

    // Add the minuend and the negated subtrahend
    bool difference[CHAR_BIT];
    bool carry2 = false;
    for (int i=0; i<CHAR_BIT; i++) {
        if (subtrahend[i] == 0 && minuend[i] == 0) {
            if (carry2) {
                difference[i] = 1;
                carry2 = false;
            } else {
                difference[i] = 0;
            }
        } else if ((subtrahend[i] == 1 && minuend[i] == 0) || (subtrahend[i] == 0 && minuend[i] == 1)) {
            if (carry2) {
                difference[i] = 0;
                //carry2 = false;
            } else {
                difference[i] = 1;
            }
        } else if (subtrahend[i] == 1 && minuend[i] == 1) {
            if (carry2) {
                difference[i] = 1;
            } else {
                difference[i] = 0;
            }
            carry2 = true;
        }
    }

    // print the difference bit string
    for (int i=CHAR_BIT-1; 0<=i; i--) {
        printf("%d",difference[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;

}
