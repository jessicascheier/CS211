#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

int main(int argc, char *argv[]) {

    // SETUP

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double value;
    fscanf(fp, "%lf", &value);
    // maybe if i use atoi and fgetc i can detect the - sign and differ 0.0 from -0.0

    // the reference solution ('the easy way')
    // you are not allowed to print from this casted 'ref_bits' variable below
    // but, it is helpful for validating your solution
    //unsigned long int ref_bits = *(unsigned long int*) &value;

    // THE SIGN BIT
    bool sign = value<0.0;
    if ((1/value) < 0) {
        sign = true;
    }
    printf("%d_",sign);
    //assert (sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ))); // validate your result against the reference

    // THE EXP FIELD
    // find the exponent E such that the fraction will be a canonical fraction:
    // 1.0 <= fraction < 2.0
    double fraction = sign ? -value : value;
    //double ok = sign ? -value : value;

    signed short trial_exp=(1<<(EXP_SZ-1))-1; // start by assuming largest possible exp (before infinity)
    // do trial division until the proper exponent E is found
    /*to express the exponent of the number 3.45678 x 10^2 in binary using an 11-bit exponent, 
    we first convert the decimal exponent 2 to binary, which is 10. 
    Then, we add the bias of 1023 to get 00000111111 + 0000000010 = 00001000101. 
    Therefore, the exponent of this number in binary form using an 11-bit exponent is 00001000101.*/
    
    /*int divisions = 0;
    unsigned int whole = fraction;
    //int temp;

    double fracbits = fraction - (int)fraction;
    int multiply = 0;
    if ((fabs(fraction) < 1.0) && fraction != 0.0) { // only for fractions, numbers to the right of the decimal point
        while (!(fracbits >= 1.0)) {
            multiply++;
            fracbits *= 2;
        } // fails no test cases
        trial_exp = -(multiply);
    } else { // only for left of decimal point, wholes
        while (whole != 0) {
            divisions++;
            whole /= 2;
        }
        trial_exp = divisions - 1;
    }*/

    int divisions = 0;
    if (fraction == 0.0) { // edge case
        trial_exp = -1023; // is it 1023 or 1022?
        // so it will print as all 0s
    } else if (fraction >= 1.0) { // if whole
        if (fraction / (pow(2, 1023)) >= 1) {
            trial_exp = 1023;
        } else {
            while (divisions < 1) {
                trial_exp = trial_exp - 1;
                double divide = pow(2, trial_exp);
                divisions = fraction / divide;
            }
        }
    } else if (fraction < 1.0) { // if fractional 
        trial_exp = 0;
        while (divisions < 1 && trial_exp > -1023) {
            trial_exp = trial_exp - 1;
            double divide = pow(2, trial_exp);
            divisions = fraction / divide;
        }
    }

    unsigned short bias = (1<<(EXP_SZ-1))-1;
    signed short exp = trial_exp + bias;
    //printf("\nexp: %d\n", exp);

    for ( int exp_index=EXP_SZ-1; 0<=exp_index; exp_index-- ) {
        bool exp_bit = 1&exp>>exp_index;
        printf("%d",exp_bit);
        //assert (exp_bit == (1&ref_bits>>(exp_index+FRAC_SZ))); // validate your result against the reference
    }
    printf("_");
    // you get partial credit by getting the exp field correct
    // even if you print zeros for the frac field next
    // you should be able to pass test cases 0, 1, 2, 3

    // THE FRAC FIELD
    // prepare the canonical fraction such that:
    // 1.0 <= fraction < 2.0
    // ... /
    double canonical;
    if (value < 1.0 && value >= 0.0) { // mantissa less than 1, but positive
        if (exp == 0) {
            canonical = (fabs(value)/(pow(2, -1022)));
        } else {
            canonical = (fabs(value) / (pow(2, trial_exp))) - 1;
        }
    } else if (value < 0.0) { // how to get mantissa of a negative decimal
        if (exp == 0 && value > -1.0) {
            canonical = (fabs(value) / (pow(2, -1022))); // works for test 20 and 22 etc
        } else {
            canonical = (fabs(value) / (pow(2, trial_exp))) - 1; // test 4
        }
    } else { // mantissa greater than 1
        canonical = (value / (pow(2, trial_exp))) - 1;
    }
    //double canonical = (value / (pow(2, trial_exp))) - 1; // do you have to subtract 
    //printf("canonical: %f\n", canonical);
    // convert to binary and done!
    
    bool frac_array[FRAC_SZ+1]; // one extra LSB bit for rounding
    for ( int frac_index=FRAC_SZ; 0 <= frac_index; frac_index-- ) {
        if (canonical*2 >= 1) {
            frac_array[frac_index] = true;
            canonical = (canonical*2) - 1; // 0.8125 * 2 = 1.625, .625 * 2 = 1.25, .25 * 2 = 0.5, 0.5 * 2 = 1
        } else {
            frac_array[frac_index] = false;
            canonical = canonical*2;
        }
        //frac_array[frac_index] = false; // frac set to zero to enable partial credit
    }

    // rounding (truncate)

    for ( int frac_index=FRAC_SZ-1; 0<=frac_index; frac_index-- ) {
        bool frac_bit = frac_array[frac_index+1]; // skipping the extra LSB bit for rounding
        printf("%d", frac_bit);
        //assert (frac_bit == (1&ref_bits>>frac_index)); // validate your result against the reference
    }

    return EXIT_SUCCESS;

}