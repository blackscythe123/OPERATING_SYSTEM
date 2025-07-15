#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#define SIZE 1024
int main()
{
    int fd[2],nread;
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
        close(fd[0]);
        int input=open("input.txt",O_RDONLY,0644);
        while ((bytes_read = read(input, buffer, SIZE)) != 0) 
        {
            write(fd[1], buffer, bytes_read);
        }
        close(fd[1]);
    }
    else if (pid==0)
    {
        close(fd[1]);
        int output=open("output.txt",O_WRONLY | O_CREAT ,0644);
        while((bytes_read=read(fd[0],buffer,SIZE))!=0)
        {
            bytes_write=write(output,buffer,bytes_read);
            if(bytes_write==-1)
            perror("write failed\n");
        }
        close(fd[0]);
    }
    else
    {
        perror("fork failed!");
        exit(2);
    }
    return 0;

}

