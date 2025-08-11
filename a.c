#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char id[10];
    int AT, BT, CT, TAT, WT, RT;
    int remainingBT;
    int queue_level;
    
}process;

typedef struct a    
{
    process *p[50];
    int count;
    int quantum;
}queue;

void sort_arrivaltime(process *p,int n)
{
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-i; j++)
        {
            if (p[j].AT>p[j+1].AT)
            {
                process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
            
        }
        
    }
    
}

void mlfq(process *p,queue *q,int totalprocess,int no_of_que)
{
    int current_time = 0;
    int completed = 0;
    while (completed < totalprocess)
    {    
        int highest_queue = 0;
        for (int i = 0; i < no_of_que; i++)
        {
            if (q[i].count > 0)
            {
                highest_queue = i;
                break;
            }
            
        }
        process *current_process = q[highest_queue].p[0];
        if(current_process->AT > current_time)
            {
                current_time = current_process->AT;
                continue;
            }
        if (current_process->RT=-1)
        
        current_process->RT=current_time - current_process->AT;

        int execute_time = (highest_queue < no_of_que -1) ?
        (current_process->remainingBT < q[highest_queue].quantum ? current_process->remainingBT : q[highest_queue].quantum)
        : current_process->remainingBT;
        
        current_process->remainingBT -=execute_time;
        current_time+= execute_time;
        if(current_process->remainingBT == 0)
        {
            current_process->CT=current_time;
            current_process->TAT = current_process->CT - current_process->AT ;
            current_process->WT  = current_process->TAT - current_process->BT;
            completed++;

            for (int i = 0; i < q[highest_queue].count -1; i++)
            {
                q[highest_queue].p[i]=q[highest_queue].p[i+1];

            }
            q[highest_queue].count--;
        }
        else
        {
            if (highest_queue < no_of_que -1 && current_process->remainingBT < q[highest_queue].quantum)
            {
                process* temp =current_process;
                for (int i = 0; i < q[highest_queue].count -1; i++)
                {
                    q[highest_queue].p[i] = q[highest_queue].p[i+1];
                }
                q[highest_queue].p[q[highest_queue].count-1] = temp;
            }
            else if (highest_queue < no_of_que -1)
            {
                current_process->queue_level++;
                q[highest_queue + 1].p[q[highest_queue+1].count++] = current_process;
                for (int i = 0; i < q[highest_queue].count -1; i++)
                {
                    q[highest_queue].p[i] = q[highest_queue].p[i+1];
                }
                q[highest_queue].count--;
            }
            
        }

    }

}
