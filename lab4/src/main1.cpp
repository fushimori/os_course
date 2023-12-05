#include <iostream>
#include "E/exp.h"
#include "GCF/gcf.h"
#include <dlfcn.h>

typedef int (*MyGcfFunc)(int, int);
typedef float (*MyExpFunc)(int);

int main() {

    int command;
    int a,b, x;
    bool switcher = true;
    while (true) {
        std::cout << "Enter command: ";
        if(!(std::cin >> command)){
            std::cout << "Finish work" << std::endl;
            break;
        }

        switch (command){
        case 0:
            switcher = switcher ? false : true;
            std::cout << "Mode changed: " << (switcher ? "First" : "Second") 
                                            << std::endl << std::endl;
            break;
        case 1:
            std::cout << "Enter a and b: ";
            std::cin >> a >> b;
            if(switcher){
                std::cout << "GCF(" << a << ", " << b << ") = " << gcf(a, b) << std::endl << std::endl;
            }
            else{
                std::cout << "GCF(" << a << ", " << b << ") = " << gcf_naive(a, b) << std::endl << std::endl;
            }
            break;
        case 2:
            std::cout << "Enter x: ";
            std::cin >> x;
            if(switcher){
                std::cout << "Exp(" << x << ") = " << my_exp(x)<< std::endl << std::endl;

            }
            else{
                std::cout << "Exp(" << x << ") = " << my_exp_series(x) << std::endl << std::endl;  
            }
            break;
        default:
            break;
        }

    }
    
    return 0;
}