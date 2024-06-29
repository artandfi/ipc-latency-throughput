// Shared Memory - Writer

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
    clock_t time;
    size_t size = (size_t)4096 * (size_t)1024;

    sem_t* sem_prod = sem_open(SEM_PROD, O_RDWR);
    sem_t* sem_cons = sem_open(SEM_CONS, O_RDWR);

    sem_wait(sem_cons);

    key_t key = ftok(SHMEM_FILE, 65);
    int shmid = shmget(key, size, 0666|IPC_CREAT);
    char* data = shmat(shmid, NULL, 0);
    time = clock();
    memset(data, 1, size);

    sem_post(sem_prod);
    sem_wait(sem_cons);
    time = clock() - time;
    //printf("%d", (int)data[0]);

    sem_close(sem_prod);
    sem_close(sem_cons);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    double t = ((double)time) / CLOCKS_PER_SEC;
    double latency = t / 2.0;
    double throughput = ((double)size) / latency / (1024*1024);
    printf("Time elapsed (W-write + R-read + R-write + W-read): %f s\n", t);
    printf("Approximate latency: %f s\n", latency);
    printf("Approximate throughput: %f MiB/s\n", throughput);
    
    return 0;
}
