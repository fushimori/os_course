#include <iostream>
#include "E/exp.h"
#include "GCF/gcf.h"
#include <dlfcn.h>

typedef int (*MyGcfFunc)(int, int);
typedef float (*MyExpFunc)(int);

int main() {

    void* my_gcf_lib = dlopen("libGCF.so", RTLD_LAZY);
    if (!my_gcf_lib) {
        std::cout << "Cannot open libGCF" << dlerror() << std::endl;
        return 1;
    }

    void* my_exp_lib = dlopen("libEXP.so", RTLD_LAZY);
    if (!my_exp_lib) {
        std::cout << "Cannot open libEXP" << dlerror() << std::endl;
        dlclose(my_gcf_lib);
        return 1;
    }


    int command;
    int a,b, x;
    bool switcher = true;
    while (true) {
        std::cout << "Enter command: ";
        if(!(std::cin >> command)){
            std::cout << "Finish work" << std::endl;
            dlclose(my_gcf_lib);
            dlclose(my_exp_lib);
            break;
        }

        switch (command){
        case 0:
            switcher = switcher ? false : true;
            std::cout << "Mode changed: " << (switcher ? "First" : "Second") 
                                            << std::endl << std::endl;
            break;
        case 1:
            MyGcfFunc gcf_func;
            if(switcher){
                gcf_func = (MyGcfFunc) dlsym(my_gcf_lib, "gcf");

            }
            else{
                gcf_func = (MyGcfFunc) dlsym(my_gcf_lib, "gcf_naive");
            }

            if (!gcf_func) {
                std::cout << "Cannot find symbol of libGCF: " << dlerror() << std::endl;
                dlclose(my_gcf_lib);
                return 1;
            }

            std::cout << "Enter a and b: ";
            std::cin >> a >> b;
            std::cout << "GCF(" << a << ", " << b << ") = " << (*gcf_func)(a, b) << std::endl << std::endl;
            break;
        case 2:
            MyExpFunc exp_func;
            if(switcher){
                exp_func = (MyExpFunc) dlsym(my_exp_lib, "my_exp");

            }
            else{
                exp_func = (MyExpFunc) dlsym(my_exp_lib, "my_exp_series");    
            }
            if (!exp_func) {
                std::cout << "Cannot find symbol of libEXP: " << dlerror() << std::endl;
                dlclose(my_exp_lib);
                return 1;
            }
            
            std::cout << "Enter x: ";
            std::cin >> x;
            std::cout << "Exp(" << x << ") = " << (*exp_func)(x) << std::endl << std::endl;
            break;
        default:
            break;
        }

    }
    
    return 0;
}