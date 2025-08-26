#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define SHM_SIZE 1024
#define SHM_KEY 0x1234

typedef struct {
    char name[SHM_SIZE];
    int ready;
} SharedData;

int main() {
    int shmid;
    SharedData *shm_ptr;
    pid_t pid;


    shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    shm_ptr = (SharedData *)shmat(shmid, NULL, 0);
    if (shm_ptr == (SharedData *)-1) {
        perror("shmat");
        exit(1);
    }

    shm_ptr->ready = 0;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) { 
        printf("Parent Process: Enter a name to convert into uppercase: ");
        fgets(shm_ptr->name, SHM_SIZE, stdin);
        shm_ptr->name[strcspn(shm_ptr->name, "\n")] = '\0'; 
        shm_ptr->ready = 1; 
        wait(NULL); 

        
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
    } else { 
        while (shm_ptr->ready == 0) {
            usleep(1000); 
        }
        int i;
        for (i = 0; shm_ptr->name[i]; i++) {
            shm_ptr->name[i] = toupper(shm_ptr->name[i]);
        }
        printf("Child process: Name in Uppercase: %s\n", shm_ptr->name);

       
        shmdt(shm_ptr);
    }
    return 0;
}
