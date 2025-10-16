#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    // Step 1: Generate unique key
    key_t key = ftok("shmfile", 65);

    // Step 2: Create shared memory segment
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // Step 3: Attach to the shared memory
    char *str = (char*) shmat(shmid, NULL, 0);

    printf("Write Data: ");
    fgets(str, 1024, stdin); // write to shared memory

    printf("Data written in memory: %s\n", str);

    // Step 4: Detach from shared memory
    shmdt(str);

    return 0;
}
