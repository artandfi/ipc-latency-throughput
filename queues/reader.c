// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	key_t key;
	int msgid;
	size_t size = (size_t)4096 * (size_t)1024;
    char* data = malloc(size);
    memset(data, 0, size);

	// ftok to generate unique key
	key = ftok("progfile", 65);

	// msgget creates a message queue
	// and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);

	// msgrcv to receive message
	msgrcv(msgid, data, size, 0, 0);
	msgsnd(msgid, data, size, 0);

	return 0;
}
