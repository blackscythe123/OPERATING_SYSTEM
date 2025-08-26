// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#define SHM_KEY 1234

struct shmseg {
    int turn;              
    char message[100];
    int terminate;
};

// Make stdin non-blocking
void set_nonblocking_stdin() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int shmid;
    struct shmseg *shmp;
    char input[100];

    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    shmp = (struct shmseg*) shmat(shmid, NULL, 0);
    if (shmp == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Client started. Type messages anytime. Waiting for server messages...\n");

    set_nonblocking_stdin();
    input[0] = '\0';

    while (!shmp->terminate) {
        // Check for server message
        if (shmp->turn == 1 && strlen(shmp->message) > 0) {
            printf("\nReceived from server: %s\n", shmp->message);
            if (strcmp(shmp->message, "bye") == 0 || strcmp(shmp->message, "exit") == 0) {
                shmp->terminate = 1;
                break;
            }
            shmp->message[0] = '\0';  // clear after reading
            shmp->turn = 0;
            printf("Client: ");
            fflush(stdout);
        }

        // Check user input
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0;  // trim newline

            if (strlen(input) > 0 && shmp->turn == 0) {
                strcpy(shmp->message, input);
                if (strcmp(input, "bye") == 0 || strcmp(input, "exit") == 0) {
                    shmp->terminate = 1;
                    break;
                }
                shmp->turn = 1;
            }
            input[0] = '\0';
        }

        usleep(100000);
    }

    shmdt((void*)shmp);
    printf("\nClient terminated.\n");
    return 0;
}
