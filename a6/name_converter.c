#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

#define SHM_SIZE 100

int main() {
    key_t key;
    int shmid;
    char *shared_memory;
    pid_t pid;
    
    // Generate a key for shared memory
    key = ftok("/tmp", 65);
    
    // Create shared memory segment
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    
    // Attach shared memory
    shared_memory = (char *)shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    pid = fork();
    
    if (pid == 0) {
        // Child process - convert to uppercase
        printf("Child process: Waiting for input from parent...\n");
        
        // Wait until parent writes the name
        while (strlen(shared_memory) == 0) {
            sleep(1);
        }
        
        printf("Child process: Converting '%s' to uppercase\n", shared_memory);
        
        // Convert to uppercase
        for (int i = 0; shared_memory[i]; i++) {
            shared_memory[i] = toupper(shared_memory[i]);
        }
        
        printf("Child process: Name in Uppercase: %s\n", shared_memory);
        
        // Detach shared memory
        shmdt(shared_memory);
        exit(0);
        
    } else if (pid > 0) {
        // Parent process - get name input
        char name[100];
        
        printf("Parent Process: Enter a name to convert into uppercase: ");
        fgets(name, sizeof(name), stdin);
        
        // Remove newline character
        name[strcspn(name, "\n")] = 0;
        
        // Write to shared memory
        strcpy(shared_memory, name);
        
        // Wait for child to complete
        wait(NULL);
        
        printf("Parent Process: Final result: %s\n", shared_memory);
        
        // Detach and remove shared memory
        shmdt(shared_memory);
        shmctl(shmid, IPC_RMID, NULL);
        
    } else {
        perror("fork failed");
        exit(1);
    }
    
    return 0;
}