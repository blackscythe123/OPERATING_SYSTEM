// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

struct shmseg {
    int turn;              // 0=client's turn, 1=server's turn
    char message[100];
    int terminate;
};

int main() {
    int shmid;
    struct shmseg *shmp;

    shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    shmp = (struct shmseg*) shmat(shmid, NULL, 0);
    if (shmp == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Server started. Waiting for client...\n");

    shmp->turn = 0;       // client starts
    shmp->terminate = 0;  
    shmp->message[0] = '\0'; // clear message buffer

    while (!shmp->terminate) {
        if (shmp->turn == 0) {
            if (strlen(shmp->message) > 0) {
                printf("Received from client: %s\n", shmp->message);
                if (strcmp(shmp->message, "bye") == 0 || strcmp(shmp->message, "exit") == 0) {
                    shmp->terminate = 1;
                    break;
                }
                shmp->message[0] = '\0'; // clear after reading
                shmp->turn = 1;
            }
        } else if (shmp->turn == 1) {
            char buff[100];
            printf("Server: ");
            fflush(stdout);
            if (fgets(buff, sizeof(buff), stdin) == NULL) {
                perror("fgets");
                break;
            }
            buff[strcspn(buff, "\n")] = 0;  // trim newline

            strcpy(shmp->message, buff);
            if (strcmp(buff, "bye") == 0 || strcmp(buff, "exit") == 0) {
                shmp->terminate = 1;
                break;
            }
            shmp->turn = 0;
        }
        usleep(100000);
    }

    shmdt((void*)shmp);
    shmctl(shmid, IPC_RMID, NULL);  // remove shared memory
    printf("Server terminated.\n");
    return 0;
}
