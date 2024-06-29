// IPC using mmap w/ parent & child processes

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    clock_t time;
    size_t size = (size_t)4096 * (size_t)1024;
    char* parent_data = malloc(size);
    char* child_data = malloc(size);
    
    memset(parent_data, 1, size);
    memset(child_data, 0, size);

    time = clock();
    void* shmem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    time = clock() - time;

    int pid = fork();
    if (pid) {
        time = clock();
        memcpy(shmem, parent_data, size);
        wait(NULL);
        memcpy(parent_data, shmem, size);
        time = clock() - time;
        
        double t = ((double)time) / CLOCKS_PER_SEC;
        double latency = t / 2.0;
        double throughput = ((double)size) / latency / (1024*1024);
        printf("Time elapsed (W-write + R-read + R-write + W-read): %f s\n", t);
        printf("Approximate latency: %f s\n", latency);
        printf("Approximate throughput: %f MiB/s\n", throughput);
    }
    else {
        memcpy(child_data, shmem, size);
        memcpy(shmem, child_data, size);
    }

    return 0;
}
