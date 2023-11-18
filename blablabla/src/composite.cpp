#include <iostream>
#include "composite.h"

int isComposite(int num){
    if(num <= 0){
        return 0;
    }
    if(num == 1){
        return 1;
    }
    for(int i = 2; i * i <= num; i++){
        if(num % i == 0){
            return 1;
        }
    }
    return 0;
}