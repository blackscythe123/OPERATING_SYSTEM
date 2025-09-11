#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define SHM_SIZE BUFFER_SIZE * sizeof(int) + 3 * sizeof(sem_t) + sizeof(int)

typedef struct {
    int buffer[BUFFER_SIZE];
    int n;
    sem_t empty;
    sem_t full;
    sem_t mutex;
    int in;
    int out;
} SharedData;

int main() {
    key_t key;
    int shmid;
    SharedData *shared_data;

    // Generate the same key as producer
    key = ftok("/tmp", 68);

    // Get existing shared memory segment
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("Consumer: shmget failed - make sure producer is running first");
        exit(1);
    }

    // Attach shared memory
    shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (SharedData *)-1) {
        perror("Consumer: shmat failed");
        exit(1);
    }

    printf("Consumer: Connected, waiting for %d numbers\n", shared_data->n);

    int consumed = 0;
    while (consumed < shared_data->n) {
        sem_wait(&shared_data->full);
        sem_wait(&shared_data->mutex);
        // Consume number
        int num = shared_data->buffer[shared_data->out];
        if (num == -1) break; // End signal
        shared_data->out = (shared_data->out + 1) % BUFFER_SIZE;
        printf("Consumer consumed: %d\n", num);
        sem_post(&shared_data->mutex);
        sem_post(&shared_data->empty);
        consumed++;
    }

    // Cleanup
    shmdt(shared_data);
    printf("Consumer: Disconnected.\n");

    return 0;
}