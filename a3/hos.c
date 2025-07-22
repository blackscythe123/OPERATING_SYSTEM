#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tokenNumber;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
} Patient;
int comp_FCFS(const void *a, const void *b) {
    return ((Patient *)a)->arrivalTime -((Patient *)b)->arrivalTime;
}
int comp_SJF(const void *a, const void *b) {
    return ((Patient *)a)->burstTime -((Patient *)b)->burstTime;
}
void FCFS(Patient p[],int n)
{
    qsort(p, n, sizeof(p[0]), comp_FCFS);
    p[0].waitingTime = 0;
    p[0].turnaroundTime = p[0].burstTime;
    p[0].responseTime = 0;
    p[0].tokenNumber=1;
    for(int i=1;i<n;i++)
    {
        p[i].tokenNumber=p[i-1].tokenNumber+1;
        p[i].waitingTime=p[i-1].turnaroundTime;
        p[i].turnaroundTime=p[i].waitingTime+p[i].burstTime;
        p[i].responseTime=p[i].waitingTime;
    }
}
void SJF(Patient p[],int n)
{
    qsort(p, n, sizeof(p[0]), comp_SJF);
    p[0].waitingTime = 0;
    p[0].turnaroundTime = p[0].burstTime;
    p[0].responseTime = 0;
    // p[0].tokenNumber=1;
    for(int i=1;i<n;i++)
    {
        // p[i].tokenNumber=p[i-1].tokenNumber+1;
        p[i].waitingTime=p[i-1].turnaroundTime;
        p[i].turnaroundTime=p[i].waitingTime+p[i].burstTime;
        p[i].responseTime=p[i].waitingTime;
    }

}
void displayResults(Patient patients[], int n, char* algorithm) {
    printf("\n%s Scheduling Algorithm\n", algorithm);
    printf("Token No.\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\tResponse Time\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               (algorithm=="FCFS") ? patients[i].tokenNumber:i+1,
               patients[i].arrivalTime,
               patients[i].burstTime,
               patients[i].waitingTime,
               patients[i].turnaroundTime,
               patients[i].responseTime);
    }


    printf("\nGantt Chart:\n");
    for(int i=0;i<n*8;i++)
    {
        printf("_");
    }
    printf("\n");
    printf("|");
    for (int i = 0; i < n; i++) {
        printf("P%d\t|", patients[i].tokenNumber);
    }
    printf("\n|");
        for(int i=0;i<n;i++)
    {
        printf("_______|");
    }
    printf("\n");
    printf("0\t");
    for (int i = 0; i < n; i++) {
        printf("%d\t", patients[i].turnaroundTime);
    }

    printf("\n");
}
int main()
{
    int n;
    printf("Enter number of paitents:");
    scanf("%d",&n);

    Patient patients[n];

    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for patient %d: ", i + 1);
        scanf("%d %d", &patients[i].arrivalTime, &patients[i].burstTime);
        patients[i].tokenNumber = i + 1;
    }
    FCFS(patients,n);
    displayResults(patients,n,"FCFS");
    SJF(patients,n);
    displayResults(patients,n,"SJF");
}