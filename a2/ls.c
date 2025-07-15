#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#define SIZE 1024
int main()
{
    int fd[2];
    char buffer[SIZE];
    pid_t pid;
    ssize_t bytes_read,bytes_write;
    // FILE* in="input.txt";
    // FILE* out="output.txt";
    if(pipe(fd)==-1)
    {
        perror("pipe creation failed!\n");
        exit(1);
    }
    pid=fork();
    if(pid>0)
    {
        wait(NULL);
        close(fd[1]);
        int output_fd = open("ls.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (output_fd == -1) {
            perror("File open failed");
            exit(1);
        }
        while ((bytes_read = read(fd[0], buffer, SIZE)) > 0) 
        {
            bytes_write=write(output_fd, buffer, bytes_read);
            if(bytes_write==-1)
            perror("write failed\n");
        }
        close(fd[0]);
        close(output_fd); //

        int output=open("ls.txt",O_RDONLY,0644);
        if (output == -1) {
            perror("Failed to reopen file for reading");
            exit(1);
        }
        while((bytes_read=read(output,buffer,SIZE))>0)
        {
            bytes_write=write(1,buffer,bytes_read);
            if(bytes_write==-1)
            perror("write failed\n");
        }
        close(output);
        
    }
    else if (pid==0)
    {
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);
        if(execl("/bin/ls","ls","-l",NULL)==-1)
        {
        perror("invalid pathway using home path");
        
        exit(1);
        }
    }
    else
    {
        perror("fork failed!");
        exit(2);
    }
    return 0;

}

