#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

void execute_ls(char *path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

void execute_cat(char *path) {
    int fd;
    char buffer[1024];
    ssize_t bytes_read;

    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("open");
        return;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(fd);
}

void execute_exit() {
    exit(0);
}

void parse_input(char *input) {
    char *command = strtok(input, " ");
    char *arg = strtok(NULL, " ");

    if (strcmp(command, "ls") == 0) {
        execute_ls(arg == NULL ? "." : arg);
    } else if (strcmp(command, "cat") == 0) {
        if (arg == NULL) {
            printf("Usage: cat <file_path>\n");
        } else {
            execute_cat(arg);
        }
    } else if (strcmp(command, "exit") == 0) {
        execute_exit();
    } else {
        printf("Unknown command: %s\n", command);
    }
}

int main() {
    char input[1024];

    while (1) {
        printf("mini_shell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets");
            exit(1);
        }

        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        parse_input(input);
    }

    return 0;
}
