#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
void ls(){
    char dict[255];
    printf("enter the directory path:");
    scanf("%s",dict);
    pid_t pid=fork();
    if(pid==0)
    {
        printf("child is running...\n");
        if(execl("/bin/ls","ls","-l",dict,NULL)==-1)
        {
        perror("invalid pathway using home path");
        execl("/bin/ls","ls","-l","/home/mtech1",NULL);   
        }
        exit(0);
    }
    else if(pid>0)
    {
        wait(NULL);
        printf("successfully system process executed\n");
    }
    else
    printf("child creation failed!\n");
}
void cat()
{
    char file[20];
    char buffer[1024];
    ssize_t bytes_read;
    printf("enter a file name:");
    scanf("%s",file);
    int fd = open(file, O_RDONLY, 0644);
    if (fd == -1) 
    {
    perror("open failed");
    }   
printf("File opened, descriptor: %d\n", fd);
    while ((bytes_read=read(fd,buffer,sizeof(buffer)))>0)
    {
        write(STDOUT_FILENO,buffer,bytes_read);
    }
    

close(fd);
}
int main()
{
    int choice;
    while(1){
    printf("1.ls\n2.cat\n3.exit\n");
    printf("enter your choice:");
    scanf("%d",&choice);    
    switch (choice)
    {
    case 1:
        ls();
        break;
    case 2:
        cat();
        break;
    case 3:
        exit(0);
        break;
    default:
        printf("service unavailable please try what we have\n");
        break;
    }
  }
    return 0;

}