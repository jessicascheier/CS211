#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    signed int input;
    fscanf(fp, "%d", &input);

    /* print bits; you will see this kind of for loop often in this assignment
    for ( int bit=sizeof(signed int)*8-1; 0<=bit; bit-- ) {
        size_t bit_val = ((1<<1)-1) & input>>bit*1; // shift and mask
        char character = bit_val ? '1' : '0';
        printf("%c",character);
    }
    printf("\n"); */ //prints binary representation of number

    // convert to hexadecimal
    //printf("%X\n", input);

    //Hexadecimal numbers use 16 values to represent a number. 
    //Numbers 0-9 are 0-9
    //Numbers 10-15 are A B C D E F
    
    if ((int) input > 0) {
        signed long int quotient;
        int i = 1, j, temp;
        char hexadecimalNumber[100];
        quotient = input;
        while (quotient != 0) {
            temp = quotient % 16;
            if(temp < 10) {
                temp = temp + 48; // 0-9 ascii value
            } else {
                temp = temp + 55; // uppercase ascii letter value
            }
            hexadecimalNumber[i++]= temp;
            quotient = quotient / 16;
        }
        int count = 0;
        char character = '0';
        for (j = i - 1; j > 0; j--) { // get length, want to add number of zeros depending on length
            count++;
        }
        if (i == 2) { // len is 2, needs 3 zeroes
            for (int i = 0; i < 3; i++) {
                printf("%c", character);
            }
        } else if (i == 3) { // needs 2 zeroes
            for (int i = 0; i < 2; i++) {
                printf("%c", character);
            }
        } else if (i == 4) { // needs 1 zero
            printf("%c", character);
        } // else, just print the thing
            for (j = i - 1; j > 0; j--) {
                printf("%c",hexadecimalNumber[j]);
            }
        printf("\n");
    } else {
        unsigned long int n = input;
        int i = 1, j, temp;
        char hexadecimalNumber[100];
        while (n != 0) {
            temp = n % 16;
            if(temp < 10) {
                temp = temp + 48; // 0-9 ascii value
            } else {
                temp = temp + 55; // uppercase ascii letter value
            }
            hexadecimalNumber[i++]= temp;
            n = n / 16;
        }
        int count = 0;
        char saveContents[4];
        for (j = 1; j < i; j++) { // prints backwards, can shorten to right length, but need to reverse it back somehow
            saveContents[count] = hexadecimalNumber[j];
            count++;
            if (count == 4) break;
        }
        for (count = 3; count >= 0; count--) {
            printf("%c", saveContents[count]);
        }
        printf("\n");
        
    }
	
    return EXIT_SUCCESS;

}
