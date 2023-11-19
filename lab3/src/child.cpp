#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "composite.h"

int main() {

    const int mem_size = 2 * sizeof(int);  
    const char* mem_name = "/mem";  

    int fd = shm_open(mem_name, O_RDWR, 0666);  
    if (fd == -1) {
        perror("shm_open child");
        exit(-1);
    }
    void* addr = mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  
    if (addr == MAP_FAILED) {
        perror("mmap child");
        exit(-1);
    }

    int* num_ptr = (int*)addr;  
    int* flag_ptr = num_ptr + 1;  

    sem_t* fmem_sem = sem_open("/fmem_semaphore", O_CREAT, 0666, 0);
    if (fmem_sem == SEM_FAILED) {
        perror("Unable to open fmem_semaphore");     
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

    
    sem_wait(fmem_sem);
    const char* fmem_name = "/filename";  
    int fd_fname = shm_open(fmem_name, O_RDWR, 0666);  
    if (fd_fname == -1) {
        perror("shm_open child (fname)");
        exit(-1);
    }

    char fname[40];
    char* faddr = (char*)mmap(0, sizeof(fname), PROT_READ, MAP_SHARED, fd_fname, 0);  
    if (faddr == MAP_FAILED) {
        perror("mmap child (fname)");
        exit(-1);
    }

    memcpy(fname, faddr, sizeof(fname));  

    int fdout = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0777);  
    if (fdout == -1) {
        perror("fdout child");
        exit(-1);
    }

    while (true) {
        sem_wait(mem_sem2);
        int num = *num_ptr;  
        
        *flag_ptr = isComposite(num);  

        if (!(*flag_ptr)) {
            sem_post(mem_sem1);
            break;
        }

        write(fdout, &num, sizeof(int));  
        sem_post(mem_sem1);

    }

    sem_close(mem_sem1);
    sem_unlink("/mem_semaphore1");
    sem_close(mem_sem2);
    sem_unlink("/mem_semaphore2");

    munmap(addr, mem_size);
    munmap(faddr, sizeof(fname));
    close(fd);
    close(fd_fname);
    shm_unlink(mem_name);
    shm_unlink(fmem_name);

    close(fdout);
    
    return 0;
}
