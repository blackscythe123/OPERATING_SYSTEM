#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];           // fd[0] - read end, fd[1] - write end
    pid_t pid;
    char write_msg[] = "Hello from child process!";
    char read_msg[100];

    // Step 1: Create pipe
    if (pipe(fd) == -1) {
        printf("Pipe failed!\n");
        return 1;
    }

    // Step 2: Create child process
    pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }

    // Step 3: Child process
    else if (pid == 0) {
        close(fd[0]); // Close reading end
        printf("Child: Writing to pipe...\n");
        write(fd[1], write_msg, strlen(write_msg) + 1);
        close(fd[1]); // Close writing end after writing
    }

    // Step 4: Parent process
    else {
        close(fd[1]); // Close writing end
        read(fd[0], read_msg, sizeof(read_msg));
        printf("Parent: Reading from pipe: %s\n", read_msg);
        close(fd[0]); // Close reading end
    }

    return 0;
}
