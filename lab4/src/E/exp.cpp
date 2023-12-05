#include "exp.h"

float my_exp(int x){
    return pow((1 + 1 / (float)x), x);
}


float my_exp_series(int x) {
    if (x < 0) {
        return 0.0;
    }
    float sum = 0.0;
    float factorial = 1.0;

    for (int n = 0; n <= x; n++) {
        sum += 1.0 / factorial;
        factorial *= (n + 1);
    }

    return sum;
}