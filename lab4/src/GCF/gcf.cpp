#include "gcf.h"

int gcf(int a, int b){
    if(a < b){
        std::swap(a, b);
    }
    if(b == 0){
        return a;
    }
    return gcf(b, a % b);
}

int gcf_naive(int a, int b){
    if(b < a){
        std::swap(a, b);
    }
    int result = 1;
    for(int i = 1; i <= a; i++){
        if(a % i == 0 && b % i == 0){
            result = i;
        }
    }
    return result;

    
}