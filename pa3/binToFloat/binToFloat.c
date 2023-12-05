#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // decode the 32-bit binary representation of an IEEE 754 single-precision float point number

    // SETUP

    // first, read the binary number representation of float point number

    //unsigned int binary = 0;
    char buff = 0;
    bool binaryRep[32];
    for (int i = EXP_SZ + FRAC_SZ; i >= 0; i--) { // read MSB first as that is what comes first in the file
        buff = fgetc(fp);
        if (buff == 49) {
            binaryRep[i] = true;
        } else {
            binaryRep[i] = false;
        }
    }

    bool zero = false;
    for (int i = EXP_SZ + FRAC_SZ; i >= 0; i--) { // if float is 0.0
        if (binaryRep[i] == 0) {
            zero = true;
        } else {
            zero = false;
            break;
        }
    }
    if (zero) {
        printf("%f\n", 0.0);
        return EXIT_SUCCESS;
    }

    // decode the sign, 
    // exponent, and 
    // mantissa portions 
    // of the floating point number to reconstruct the number's value

    // float number = *(float *)&binary; // you are not allowed to do this.

    // S, sign 0 -- positive, 1 -- negative
    bool negative = false;
    if (binaryRep[EXP_SZ + FRAC_SZ] != 0) {
        negative = true;
    }

    // E, exponent
    int j = 0;
    bool exponent[8];
    for (int i = EXP_SZ + FRAC_SZ - 1; i >= 23; i--) { // extracts exponent as a binary representation
        // read exponent as binary number, then convert to unsigned int and subtract 127 as its the offset
        if (binaryRep[i] == true) {
            exponent[j] = true;
        } else {
            exponent[j] = false;
        }
        j++;
    }

    int base = 0;
    int exponentDecimal = 0; // test1 should yield 127, test3 is 126-127 = -1
    for (int k = 7; k >= 0; k--) { // convert to decimal, then subtract 127
        exponentDecimal += (exponent[k] * pow(2, base));
        base++;
    }
    exponentDecimal = exponentDecimal - 127; // raise 2 to the power of this exponent
    double exponentDecoded;
    if (exponentDecimal < 0) {
        exponentDecoded = 1/(pow(2, fabs(exponentDecimal)));
    } else {
        exponentDecoded = pow(2, exponentDecimal);
    }

    // M, mantissa
    int m = 0;
    bool fraction[23];
    for (int i = 22; i >= 0; i--) { // extract 23 fraction bits
        if (binaryRep[i] == true) {
            fraction[m] = true;
        } else {
            fraction[m] = false;
        }
        m++;
    }

    // to get fraction, do ( 1 + ( every bit * 2^i) )
    float frac = 0.0;
    int negativeExp = -1;
    for (int i = 22; i >= 0; i--) {
        frac += (fraction[22 - i] * (1/pow(2, fabs(negativeExp))));
        negativeExp -= 1;
    }
    frac = frac + 1;

    if (negative) {
        printf("%f\n", -(frac*exponentDecoded));
    } else {
        printf("%f\n", (frac*exponentDecoded));
    }

    // https://www.tutorialspoint.com/c_standard_library/c_function_ldexp.htm
    /*double number = ldexp ( m, e );
    number = sign ? -number : number;
    printf("%f\n", number);*/

    return EXIT_SUCCESS;

}
