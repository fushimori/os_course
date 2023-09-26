#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
// #include "composite.h"
// #define _R_            "\x1b[31m" // red color for child process output color
// #define _G_            "\x1b[32m" // green color for parent process output color

int create_process(){
    pid_t pid = fork();
    if( pid == -1){
        perror("fork");
        exit(-1);
    }
    return pid;
};

void create_pipe(int* fd){
    if(pipe(fd) == -1){
        perror("pipe");
        exit(-1);
    }
}


int main(){

    int fd1[2],fd2[2];
    create_pipe(fd1);
    create_pipe(fd2);

    pid_t pid = create_process();
    if(pid == 0){
        close(fd1[1]);
        close(fd2[0]);

        ;
        if (dup2(fd1[0], STDIN_FILENO) == -1) {
            perror("dup2 in");
            exit(-1);
        }
        if (dup2(fd2[1], STDOUT_FILENO) == -1) {
            perror("dup2 out ");
            exit(-1);
        }
        close(fd1[0]);
        close(fd2[1]);

        execl("../build/child", "../build/child", NULL);
        
    }
    else{
        close(fd1[0]);
        close(fd2[1]);

        char fname[40];
        printf("Enter file name:\n");
        scanf("%39s", fname);

        if (write(fd1[1], fname, sizeof(fname)) == 0) {
            perror("parent filename write");
            exit(-1);
        }

        int num;
        int is_composite;
        printf("Enter num:\n");
        while (scanf("%d", &num) != EOF) {
            if (write(fd1[1], &num, sizeof(int)) == 0) {
                perror("parent write");
                break;
            }

            if(read(fd2[0], &is_composite, sizeof(int)) == 0) {
                perror("parent read");
                break;
            }
            if (!is_composite) {
                printf("Finish processes\n");
                break;
            }
        }
        
        close(fd1[1]);
        close(fd2[0]);
    }
    return 0;
}