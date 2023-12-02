#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>   
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>

int create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    return pid;
}

int main() {
    const int mem_size = 2 * sizeof(int); 
    const char* mem_name = "/mem"; 

    int fd = shm_open(mem_name, O_CREAT | O_RDWR, 0666); 
    if (fd == -1) {
        perror("shm_open");
        exit(-1);
    }
    ftruncate(fd, mem_size); 
    void* addr = mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }

    int pid = create_process();
    if (pid == 0) {

        execl("./child", "./child", NULL);
        perror("execl"); 
        exit(-1);

    } else {

        int* num_ptr = (int*)addr; 
        int* flag_ptr = num_ptr + 1; 

        char fname[40];
        printf("Enter file name:\n");
        scanf("%39s", fname);

        const char* fmem_name = "/filename"; 

        int fd_fname = shm_open(fmem_name, O_CREAT | O_RDWR, 0666); 
        if (fd_fname == -1) {
            perror("shm_open (fname)");
            exit(-1);
        }
        ftruncate(fd_fname, sizeof(fname)); 
        char* faddr = (char*)mmap(0, sizeof(fname), PROT_READ | PROT_WRITE, MAP_SHARED, fd_fname, 0);
        if (faddr == MAP_FAILED) {
            perror("mmap (fname)");
            exit(-1);
        }

        sem_t* fmem_sem = sem_open("/fmem_semaphore", O_CREAT, 0666, 0);
        if (fmem_sem == SEM_FAILED) {
            perror("Unable to create fmem_semaphore");
            exit(-1);
        }
        sem_t* mem_sem1 = sem_open("/mem_semaphore1", O_CREAT, 0666, 0);
        if (mem_sem1 == SEM_FAILED) {
            perror("Unable to create mem_semaphore1");
            exit(-1);
        }
        sem_t* mem_sem2 = sem_open("/mem_semaphore2", O_CREAT, 0666, 0);
        if (mem_sem2 == SEM_FAILED) {
            perror("Unable to create mem_semaphore2");
            exit(-1);
        }

        memcpy(faddr, fname, sizeof(fname));
        sem_post(fmem_sem);
        
        int num;
        printf("Enter num:\n");
        while (scanf("%d", &num) != EOF) {
        
            *num_ptr = num; 

            sem_post(mem_sem2);
            sem_wait(mem_sem1);

            if (!(*flag_ptr)){
                printf("Finish processes\n");
                break;
            }
        }

        munmap(faddr, sizeof(fname));
        close(fd_fname);
        shm_unlink(fmem_name);

        sem_close(mem_sem1);
        sem_unlink("/mem_semaphore1");
        sem_close(mem_sem2);
        sem_unlink("/mem_semaphore2");
        sem_close(fmem_sem);
        sem_unlink("/fmem_semaphore");
    }

    munmap(addr, mem_size);
    close(fd);
    shm_unlink(mem_name);

    return 0;
}
