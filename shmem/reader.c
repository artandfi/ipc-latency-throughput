// Shared Memory - Reader

#define SEM_PROD "/sem_prod"
#define SEM_CONS "/sem_cons"
#define SHMEM_FILE "/tmp/shmem"
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
#include <time.h>
#include <sys/shm.h>

int main() {
    size_t size = (size_t)4096 * (size_t)1024;

    sem_unlink(SEM_PROD);
    sem_unlink(SEM_CONS);
    sem_t* sem_prod = sem_open(SEM_PROD, O_RDWR | O_CREAT, 0660, 0);
    sem_t* sem_cons = sem_open(SEM_CONS, O_RDWR | O_CREAT, 0660, 1);

    sem_wait(sem_prod);

    key_t key = ftok(SHMEM_FILE, 65);
    int shmid = shmget(key, size, 0666);
    char* data = shmat(shmid, NULL, 0);
    //printf("%d", (int)data[0]);
    memset(data, 0, size);
    
    sem_post(sem_cons);
    
    sem_close(sem_prod);
    sem_close(sem_cons);
    shmdt(data);
    
    return 0;
}
