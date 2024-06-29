#define SEM_PROD "/sem_prod"
#define SEM_CONS "/sem_cons"
#define MMAP_FILE "mmap_file"

#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    size_t size = (size_t)4096 * (size_t)1024;
    char* data = malloc(size);
    memset(data, 0, size);

    sem_unlink(SEM_PROD);
    sem_unlink(SEM_CONS);
    sem_t* sem_prod = sem_open(SEM_PROD, O_RDWR | O_CREAT, 0660, 0);
    sem_t* sem_cons = sem_open(SEM_CONS, O_RDWR | O_CREAT, 0660, 1);
    
    sem_wait(sem_prod);
    
    int fd = open(MMAP_FILE, O_RDWR, 0600);
    void* shmem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memcpy(data, shmem, size);
    memcpy(shmem, data, size);
    
    sem_post(sem_cons);

    sem_close(sem_prod);
    sem_close(sem_cons);
}