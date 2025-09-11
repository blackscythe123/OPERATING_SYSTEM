#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define BUFFER_SIZE 1
#define SHM_SIZE sizeof(char) + 3 * sizeof(sem_t)

typedef struct {
    char buffer[BUFFER_SIZE];
    sem_t empty;
    sem_t full;
    sem_t mutex;
} SharedData;

int main() {
    key_t key;
    int shmid;
    SharedData *shared_data;
    pid_t pid;
    char input[] = "HELLO";

    // Generate a key for shared memory
    key = ftok("/tmp", 67);

    // Create shared memory segment
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach shared memory
    shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (SharedData *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize semaphores (pshared=1 for inter-process)
    sem_init(&shared_data->empty, 1, 1); // Buffer empty initially
    sem_init(&shared_data->full, 1, 0);  // No items initially
    sem_init(&shared_data->mutex, 1, 1); // Mutex unlocked

    pid = fork();
    if (pid == 0) {
        // Child process - Consumer
        for (int i = 0; i < strlen(input); i++) {
            printf("Consumer acquired semaphore Full\n");
            sem_wait(&shared_data->full);
            printf("Consumer acquired semaphore Mutex\n");
            sem_wait(&shared_data->mutex);
            // Consume item
            printf("Consumer consumed item %c\n", shared_data->buffer[0]);
            sem_post(&shared_data->mutex);
            printf("Consumer released semaphore Mutex\n");
            sem_post(&shared_data->empty);
            printf("Consumer released semaphore empty\n");
        }
        printf("Consumer exited\n");
        // Detach shared memory
        shmdt(shared_data);
        exit(0);
    } else if (pid > 0) {
        // Parent process - Producer
        for (int i = 0; i < strlen(input); i++) {
            printf("Producer acquired semaphore Empty\n");
            sem_wait(&shared_data->empty);
            printf("Producer acquired semaphore Mutex\n");
            sem_wait(&shared_data->mutex);
            // Produce item
            shared_data->buffer[0] = input[i];
            printf("Producer produced the item: %c\n", input[i]);
            sem_post(&shared_data->mutex);
            printf("Producer released semaphore Mutex\n");
            sem_post(&shared_data->full);
            printf("Producer released semaphore full\n");
        }
        printf("Producer exited\n");
        // Wait for child to complete
        wait(NULL);
        // Destroy semaphores
        sem_destroy(&shared_data->empty);
        sem_destroy(&shared_data->full);
        sem_destroy(&shared_data->mutex);
        // Detach and remove shared memory
        shmdt(shared_data);
        shmctl(shmid, IPC_RMID, NULL);
    } else {
        perror("fork failed");
        exit(1);
    }
    return 0;
}