#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // Step 1: Generate same key
    key_t key = ftok("shmfile", 65);

    // Step 2: Locate shared memory segment
    int shmid = shmget(key, 1024, 0666);

    // Step 3: Attach to shared memory
    char *str = (char*) shmat(shmid, NULL, 0);

    printf("Data read from memory: %s\n", str);

    // Step 4: Detach and destroy
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
