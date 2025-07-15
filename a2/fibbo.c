#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
void fibbo(int n)
{
    long a=-1,b=1;
    long sum=0;
    for(int i=0;i<n;i++)
    {
        printf("%ld\t",sum);
        a=b;
        b=sum;
        sum=a+b;
    }
    printf("\n");
}
int main()
{
    int fd[2],n,number;
    pid_t pid;
    if(pipe(fd)==-1)
    {
        perror("pipe creation failed!\n");
        exit(1);
    }
    pid=fork();
    if(pid>0)
    {
        printf("enter the number of terms in fibbonacci series:");
        scanf("%d",&n);
        close(fd[0]);
        write(fd[1],&n,sizeof(int));
        close(fd[1]);
    }
    else if (pid==0)
    {
        close(fd[1]);
        read(fd[0],&number,sizeof(int));
        // printf("number=%d\n",number);
        fibbo(number);
        close(fd[0]);
    }
    else
    {
        perror("fork failed!");
        exit(2);
    }
    return 0;
    

}

