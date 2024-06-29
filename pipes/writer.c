// Named Pipes - Writer

#define SEM_PROD "/sem_prod"
#define SEM_CONS "/sem_cons"
#define PIPE_FILE "/tmp/pipe"
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>

int main() {
    clock_t time;
    size_t size = (size_t)4096 * (size_t)1024;
    char* data = malloc(size);
    memset(data, 1, size);

    sem_t* sem_prod = sem_open(SEM_PROD, O_RDWR);
    sem_t* sem_cons = sem_open(SEM_CONS, O_RDWR);

    sem_wait(sem_cons);

    mkfifo(PIPE_FILE, 0666);
    int fd = open(PIPE_FILE, O_RDWR);
    fcntl(fd, 1031, size);
    time = clock();
    write(fd, data, size);

    sem_post(sem_prod);
    sem_wait(sem_cons);

    read(fd, data, size);
    time = clock() - time;

    close(fd);
    unlink(PIPE_FILE);
    sem_close(sem_prod);
    sem_close(sem_cons);

    double t = ((double)time) / CLOCKS_PER_SEC;
    double latency = t / 2.0;
    double throughput = ((double)size) / latency / (1024*1024);
    printf("Time elapsed (W-write + R-read + R-write + W-read): %f s\n", t);
    printf("Approximate latency: %f s\n", latency);
    printf("Approximate throughput: %f MiB/s\n", throughput);
    
    return 0;
}
