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
        while ((bytes_read = read(fd[0], buffer, SIZE)) != 0) 
        {
            bytes_write=("ls.txt", buffer, bytes_read);
            if(bytes_write==-1)
            perror("write failed\n");
        }
        close(fd[0]);
        int output=open("ls.txt",O_RDONLY,0644);
        while((bytes_read=read(output,buffer,SIZE)>0))
        {
            bytes_write=(1,buffer,bytes_read);
            if(bytes_write==-1)
            perror("write failed\n");
        }
    }
    else if (pid==0)
    {
        close(fd[0]);
        dup2(fd[1],1);
        printf("dup2 success\n");
        if(execl("/bin/ls","ls","-l",NULL)==-1)
        {
        perror("invalid pathway using home path");
        }
        close(fd[1]);
    }
    else
    {
        perror("fork failed!");
        exit(2);
    }
    return 0;

}

