#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
// https://www.tutorialspoint.com/c_standard_library/limits_h.htm
#include <limits.h>
// https://www.cplusplus.com/reference/cfloat/
#include <float.h>

#define FLOAT_SZ sizeof(float)*CHAR_BIT
#define EXP_SZ (FLOAT_SZ-FLT_MANT_DIG)
#define FRAC_SZ (FLT_MANT_DIG-1)

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the binary number representation of multiplier
    bool multiplier[FLOAT_SZ];
    char buff;
    for (int i = FLOAT_SZ - 1; i >= 0 ; i--) {
        buff = fgetc(fp);
        if (buff == '\n') {
            break;
        } else if (buff == 49) {
            multiplier[i] = true;
        } else {
            multiplier[i] = false;
        }
        //printf("%d", multiplier[i]);
    }

    fscanf(fp, "%c", &buff);

    // second, read the binary number representation of multiplicand
    bool multiplicand[FLOAT_SZ];
    char buffer;
    for (int i = FLOAT_SZ - 1; i >= 0; i--) {
        buffer = fgetc(fp);
        if (buffer == 49) {
            multiplicand[i] = true;
        } else {
            multiplicand[i] = false;
        }
    }

    // converting both binaries to floats

    // getting exponents
    int multiplier_exponent = 0;
    int j = 0;
    for(int i = 23; i < FLOAT_SZ - 1; i++){
        if (multiplier[i] == true){
            multiplier_exponent += pow(2, j);
        }
        j++;
    }
    int multiplicand_exponent = 0;
    j = 0;
    for(int i = 23; i < FLOAT_SZ-1; i++){
        if (multiplicand[i] == true){
            multiplicand_exponent += pow(2, j);
        }
        j++;
    }
    multiplier_exponent -= 127;
    multiplicand_exponent -= 127;

    // getting mantissas
    long double multiplier_mantissa = 0;
    int negativeExpo = -1;
    for (int i = 22; i >= 0; i--) {
        if (multiplier[i] == true) {
            multiplier_mantissa += pow(2, negativeExpo); 
        }
        negativeExpo--;
    }
    negativeExpo = -1;
    long double multiplicand_mantissa = 0;
    for (int i = 22; i >= 0; i--) {
        if (multiplicand[i] == true) {
            multiplicand_mantissa += pow(2, negativeExpo); 
        }
        negativeExpo--;
    }

    // getting signs
    // wikipedia page was really helpful for the equations
    // https://en.wikipedia.org/wiki/Single-precision_floating-point_format

    // change sign thing
    int multiplier_sign = 0;
    int multiplicand_sign = 0;
    if (multiplier[FLOAT_SZ-1] == true) {
        multiplier_sign = 1;
    }
    if (multiplicand[FLOAT_SZ-1] == true) {
        multiplicand_sign = 1;
    }

    // getting floats using equation
    long double multiplier_float = 0;
    if (multiplier_exponent != -127) {
        multiplier_float = pow(-1, multiplier_sign) * (1 + multiplier_mantissa) * pow(2, multiplier_exponent);
    }
    long double multiplicand_float = 0;
    if (multiplicand_exponent != -127) {
        multiplicand_float = pow(-1, multiplicand_sign) * (1 + multiplicand_mantissa) * pow(2, multiplicand_exponent);
    }

    // multiply floats together
    long double product = multiplier_float*multiplicand_float;

    // convert product to 32-bit binary, use doubleToBin code
    bool sign = product < 0.0;
    if ((1/product) < 0) {
        sign = true;
    }
    if (product < 0) {
        sign = true;
        product *= -1;
    }
    printf("%d_", sign);

    long double fraction = product; // fraction
    signed short trial_exp=(1<<(EXP_SZ-1))-1;

    int divisions = 0;
    if (fraction == 0.0) { // edge case
        trial_exp = -127; // is it 1023 or 1022?
        // so it will print as all 0s
    } else if (fraction >= 1.0) { // if whole
        if (fraction / (pow(2, 127)) >= 1) {
            trial_exp = 127;
        } else {
            while (divisions < 1) {
                trial_exp = trial_exp - 1;
                double divide = pow(2, trial_exp);
                divisions = fraction / divide;
            }
        }
    } else if (fraction < 1.0) { // if fractional 
        trial_exp = 0;
        while ((fraction < 1 || fraction >= 2) && trial_exp > -127) {
            fraction = product;
            fraction /= pow(2, trial_exp - 1);
            trial_exp--;
        }
    }

    unsigned short bias = (1<<(EXP_SZ-1))-1;
    signed short exp = trial_exp + bias;

    for ( int exp_index=EXP_SZ-1; 0<=exp_index; exp_index-- ) {
        bool exp_bit = 1&exp>>exp_index;
        printf("%d",exp_bit);
        //assert (exp_bit == (1&ref_bits>>(exp_index+FRAC_SZ))); // validate your result against the reference
    }
    printf("_");

    long double canonical;
    if (product < 1.0 && product >= 0.0) { // mantissa less than 1, but positive
        if (exp == 0) {
            canonical = (fabs(product)/(pow(2, -126)));
        } else {
            canonical = (fabs(product) / (pow(2, trial_exp))) - 1;
        }
    } else if (product < 0.0) { // how to get mantissa of a negative decimal
        if (exp == 0 && product > -1.0) {
            canonical = (fabs(product) / (pow(2, -126))); // works for test 20 and 22 etc
        } else {
            canonical = (fabs(product) / (pow(2, trial_exp))) - 1; // test 4
        }
    } else { // mantissa greater than 1
        canonical = (product / (pow(2, trial_exp))) - 1;
    }
    
    int frac_array[FRAC_SZ+1]; // one extra LSB bit for rounding
    for ( int frac_index=FRAC_SZ; 0<=frac_index; frac_index-- ) {
        frac_array[frac_index] = 0;
    }
    for ( int frac_index=FRAC_SZ; 0<=frac_index; frac_index-- ) {
        if(product == 0){
            break;
        }
        if (canonical*2 >= 1) {
            frac_array[frac_index] = true;
            canonical = (canonical*2) - 1; // 0.8125 * 2 = 1.625, .625 * 2 = 1.25, .25 * 2 = 0.5, 0.5 * 2 = 1
        } else {
            frac_array[frac_index] = false;
            canonical = canonical*2;
        }
    }

    // rounding - add one logic
    if (frac_array[1] == false && frac_array[0] == true) {
        frac_array[1] = true;
    }else if (frac_array[0] == true && frac_array[1] == true) {
        for(int i = 0; i < FRAC_SZ + 1; i++) {
            if (frac_array[i] == true) {
                frac_array[i] = false;
            } else {
                frac_array[i] = true;
                break;
            }
        }
    }

    for ( int frac_index=FRAC_SZ-1; 0<=frac_index; frac_index-- ) {
        int frac_bit = frac_array[frac_index+1]; // skipping the extra LSB bit for rounding
        printf("%d", frac_bit);
    }

    return(EXIT_SUCCESS);

}

    // convert multiplier and multiplicand into floats

    /*
    int temp = 0;

    if (fraction == 0.0) { // edge case
        trial_exp = -1023; // is it 1023 or 1022?
    } else if (fraction >= 1.0) { // if whole
        if (fraction/(pow(2,1023)) >= 1) { // catch big numbers
            //printf("flag");
            trial_exp = 1023;
        } else {
            while (temp < 1) {
                trial_exp = trial_exp - 1;
                double divide = pow(2, trial_exp);
                temp = fraction / divide;
            }
        }
    } else if (fraction < 1.0) { // if fractional 
        trial_exp = 0;
        while (temp < 1 && trial_exp > -1023) {
            trial_exp = trial_exp - 1;
            double divide = pow(2, trial_exp);
            temp = fraction / divide;
        }
    }*/

    // float product = *(float *) &multiplier * *(float *) &multiplicand; // you are not allowed to print from this.
    // unsigned int ref_bits = *(unsigned int *) &product; // you are not allowed to print from this. But you can use it to validate your solution.

    // SIGN
    /*bool sign = false;
    int candSign = multiplicand[0];
    int iplierSign = multiplier[0];
    if ((candSign + iplierSign) % 2 == 1) {
        sign = true;
    }
    printf("%d_",sign);
    // assert (sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ)));

    // EXP
    // get the exp field of the multiplier and multiplicand
    bool expMultiplier[EXP_SZ];
    bool expMultiplicand[EXP_SZ];
    int j = 1;
    for (int i = 0; i < EXP_SZ; i++) {
        expMultiplier[i] = multiplier[j];
        expMultiplicand[i] = multiplicand[j];
        j++;
    }
    // add the two exp together
    int base = 1;
    int decimalExp = 0;
    for (int i = EXP_SZ - 1; i >= 0; i--) {
        decimalExp = decimalExp + expMultiplier[i]*base;
        base = base*2;
    }
    //printf("\ndecimal exponent 1: %d", decimalExp);
    int base2 = 1;
    int decimalExp2 = 0;
    for (int i = EXP_SZ - 1; i >= 0; i--) {
        decimalExp2 = decimalExp2 + expMultiplicand[i]*base2;
        base2 = base2*2;
    }
    //printf("\ndecimal exponent 2: %d", decimalExp2);

    // subtract bias - 127
    //int sum = (decimalExp + decimalExp2) - (2*127);
    signed short trial_exp = decimalExp + decimalExp2;
    signed short exp = (decimalExp + decimalExp2) - (127);
    printf("\nexp: %d\n", exp);
    //printf("\nsum: %d", sum);
    //printf("\n");
    // if it's still greater than 255, we have a problem or equal to max like all 1s?
    // convert to binary
    //don't convert to binary until after frac 

    // FRAC
    // get the frac field of the multiplier and multiplicand
    bool fracMultiplier[FRAC_SZ];
    bool fracMultiplicand[FRAC_SZ];
    int k = 9;
    for (int i = 0; i < FRAC_SZ; i++) {
        fracMultiplier[i] = multiplier[k];
        fracMultiplicand[i] = multiplicand[k];
        k++;
    }

    // assuming that the input numbers are normalized floating point numbers, 
    // add back the implied leading 1 in the mantissa

    // multiply the mantissas
    float mantissa1 = 0.0;
    int negativeExp1 = -1;
    for (int i = 22; i >= 0; i--) {
        mantissa1 += (fracMultiplier[22 - i] * pow(2.0, negativeExp1));
        negativeExp1 -= 1;
    }
    mantissa1 = mantissa1 + 1;

    float mantissa2 = 0.0;
    int negativeExp2 = -1;
    for (int i = 22; i >= 0; i--) {
        mantissa2 += (fracMultiplicand[22 - i] * pow(2.0, negativeExp2));
        negativeExp2 -= 1;
    }
    mantissa2 = mantissa2 + 1;
    
    float product = (mantissa1 * mantissa2) * pow(2, exp);
    printf("%f\n", mantissa1);
    printf("%f\n", mantissa2);
    printf("product:%f\n", product);

    // overflow and normalize
    double canonical;
    if (product < 1.0 && product >= 0.0) { // mantissa less than 1, but positive
        if (exp == 0) {
            canonical = (fabs(product)/(pow(2, -126)));
        } else {
            canonical = (fabs(product) / (pow(2, trial_exp))) - 1;
        }
    } else if (product < 0.0) { // mantissa of a negative decimal
        if (exp == 0 && product > -1.0) {
            canonical = (fabs(product) / (pow(2, -126))); 
        } else {
            canonical = (fabs(product) / (pow(2, trial_exp))) - 1;
        }
    } else { // mantissa > 1
        canonical = (product / (pow(2, trial_exp))) - 1;
    }*/

    // rounding

    // if M >= 2, shift M right & increment exponent
    // if exponent out of range, overflow
    // round M to fit frac precision
    
    // correct exponent: 4
    // off by one: 20, 18, 17, 16, 14, 13, 12, 9, 7, 5
    // completely different: 8
    // fails tests 

    // move decimal point

    // print exp
    /*bool exponent[EXP_SZ];
    int i;
    for (i = EXP_SZ - 1; i >= 0; i--) {
        if (exp == 0) {
            break;
        }
        exponent[i] = exp % 2;
        exp = exp / 2;
    }
    while (i >= 0) {
        exponent[i] = false;
        i--;
    }
    for (int i = 0; i < EXP_SZ; i++) {
        printf("%d", exponent[i]);
    }
    printf("_");*/