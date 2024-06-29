// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    clock_t time;
	key_t key;
	int msgid;
	size_t size = (size_t)4096 * (size_t)1024;
    char* data = malloc(size);
    memset(data, 1, size);

	key = ftok("progfile", 65);

	msgid = msgget(key, 0666 | IPC_CREAT);

	time = clock();
	msgsnd(msgid, data, size, 0);
    msgrcv(msgid, data, size, 0, 0);
	time = clock() - time;
	msgctl(msgid, IPC_RMID, NULL);

	double t = ((double)time) / CLOCKS_PER_SEC;
    double latency = t / 2.0;
    double throughput = ((double)size) / latency / (1024*1024);
    printf("Time elapsed (W-write + R-read + R-write + W-read): %f s\n", t);
    printf("Approximate latency: %f s\n", latency);
    printf("Approximate throughput: %f MiB/s\n", throughput);

	return 0;
}
