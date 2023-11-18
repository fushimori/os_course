#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "composite.h"

// #define _R_            "\x1b[31m" // red color for child process output color
// #define _G_            "\x1b[32m" // green color for parent process output color

int main() {
    char fname[40];

    if(read(STDIN_FILENO, fname, sizeof(fname)) == 0){
        exit(-1);
    }
    int fdout = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fdout == -1) {
        perror("fdout");
        exit(-1);
    }

    int num;
    while (read(STDIN_FILENO, &num, sizeof(int))) {
        int flag = isComposite(num);
        if(flag){
            if(write(fdout, &num, sizeof(int)) == 0){
                perror("fdout write");
                break;
            }
            if(write(STDOUT_FILENO, &flag, sizeof(int)) == 0){
                perror("stdout write");
                break;
            }
        }
        else{
            if(write(STDOUT_FILENO, &flag, sizeof(int)) == 0){
                perror("stdout write");
                break;
            }
            break;
        }
    }

    close(fdout);
    return 0;
}