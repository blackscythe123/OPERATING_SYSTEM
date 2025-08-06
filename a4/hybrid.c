#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h> 
#define sleep(ms) Sleep(ms * 1000) 
#else
#include <unistd.h> 
#endif

#define MAX_GANTT_EVENTS 1000 
typedef struct Process {
    char id[50];
    int AT, BT, CT, TAT, WT, RT;
    int remainingBT;
    int queueLevel; 
} P;

typedef struct Queue {
    P* arr[50]; 
    int count;
    int quantum; 
} Q;

typedef struct GanttEvent {
    char pid[50];
    int start;
    int end;
} GanttEvent;

GanttEvent ganttLog[MAX_GANTT_EVENTS];
int ganttCount = 0;

void SortProcessesByAT(P *processes, int totalProcesses) {

    for (int i = 0; i < totalProcesses - 1; i++) {
        for (int j = 0; j < totalProcesses - i - 1; j++) {
            if (processes[j].AT > processes[j + 1].AT) {
                P temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

int GetNextArrival(P *processes, int totalProcesses, int completed, int currentTime) {
    int nextAT = -1;
    for (int i = 0; i < totalProcesses; i++) {
        if (processes[i].remainingBT > 0 && processes[i].AT > currentTime) {
            if (nextAT == -1 || processes[i].AT < nextAT) {
                nextAT = processes[i].AT;
            }
        }
    }
    return nextAT;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void logGanttEvent(const char* pid, int start, int end) {
    if (ganttCount < MAX_GANTT_EVENTS) {
        strcpy(ganttLog[ganttCount].pid, pid);
        ganttLog[ganttCount].start = start;
        ganttLog[ganttCount].end = end;
        ganttCount++;
    }
}

void printGanttChart() {
    clearScreen(); 

    int i;
    printf("+");
    for (i = 0; i < ganttCount; i++) printf("-------+");
    printf("\n");

    printf("|");
    for (i = 0; i < ganttCount; i++) {
        printf("  %-3s  |", ganttLog[i].pid
    }
    printf("\n");

 
    printf("+");
    for (i = 0; i < ganttCount; i++) printf("-------+");
    printf("\n");

    printf("%-7d", ganttLog[0].start);
    for (i = 0; i < ganttCount; i++) {
        printf("%-7d", ganttLog[i].end);
    }
    printf("\n");
    printf("+");
    for (i = 0; i < ganttCount; i++) printf("-------+");
    printf("\n");

    sleep(1); 

void MLFQScheduling(Q *queues, int numQueues, P *processes, int totalProcesses) {
    int currentTime = 0;
    int completed = 0;

    while (completed < totalProcesses) {
        int highestQueue = -1;
        for (int q = 0; q < numQueues; q++) {
            if (queues[q].count > 0) {
                highestQueue = q;
                break;
            }
        }

        if (highestQueue == -1) {
            int nextAT = GetNextArrival(processes, totalProcesses, completed, currentTime);
            if (nextAT != -1) {
                currentTime = nextAT; 
            } else {
                currentTime++; 
            }
            continue;
        }

        P *current = queues[highestQueue].arr[0]; 

        if (current->AT > currentTime) {
            currentTime = current->AT;
            continue;
        }

        if (current->RT == -1) current->RT = currentTime - current->AT; 

        int sliceStart = currentTime; 
        int execTime = (highestQueue < numQueues - 1) ? 
                       (current->remainingBT < queues[highestQueue].quantum ? current->remainingBT : queues[highestQueue].quantum) 
                       : current->remainingBT; 

        current->remainingBT -= execTime;
        currentTime += execTime;

        logGanttEvent(current->id, sliceStart, currentTime);
        printGanttChart();

        if (current->remainingBT == 0) {
            current->CT = currentTime;
            current->TAT = current->CT - current->AT;
            current->WT = current->TAT - current->BT;
            completed++;

            for (int i = 0; i < queues[highestQueue].count - 1; i++) {
                queues[highestQueue].arr[i] = queues[highestQueue].arr[i + 1];
            }
            queues[highestQueue].count--;
        } else {
           
            if (highestQueue < numQueues - 1 && execTime < queues[highestQueue].quantum) {
                
                P *temp = current;
                for (int i = 0; i < queues[highestQueue].count - 1; i++) {
                    queues[highestQueue].arr[i] = queues[highestQueue].arr[i + 1];
                }
                queues[highestQueue].arr[queues[highestQueue].count - 1] = temp;
            } else if (highestQueue < numQueues - 1) {
                current->queueLevel++;
                queues[highestQueue + 1].arr[queues[highestQueue + 1].count++] = current;
                for (int i = 0; i < queues[highestQueue].count - 1; i++) {
                    queues[highestQueue].arr[i] = queues[highestQueue].arr[i + 1];
                }
                queues[highestQueue].count--;
            }
        }
    }
}

void DisplayResults(P *processes, int totalProcesses) {
    printf("\nProcess Scheduling Results:\n");
    printf("ID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < totalProcesses; i++) {
        P p = processes[i];
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\n", p.id, p.AT, p.BT, p.CT, p.TAT, p.WT, p.RT);
    }
}

void CalculatePerformance(P *processes, int totalProcesses) {
    float avgTAT = 0, avgWT = 0, avgRT = 0;
    for (int i = 0; i < totalProcesses; i++) {
        avgTAT += processes[i].TAT;
        avgWT += processes[i].WT;
        avgRT += processes[i].RT;
    }
    printf("\nPerformance Metrics:\n");
    printf("Average Turnaround Time: %.2f\n", avgTAT / totalProcesses);
    printf("Average Waiting Time: %.2f\n", avgWT / totalProcesses);
    printf("Average Response Time: %.2f\n", avgRT / totalProcesses);
}

int main(void) {
    int totalProcesses = 0;
    P processes[50];
    Q queues[3]; 

    queues[0].quantum = 4; 
    queues[1].quantum = 8; 
    queues[2].quantum = 0; 
    for (int i = 0; i < 3; i++) {
        queues[i].count = 0;
    }

    printf("Enter the total number of processes: ");
    scanf("%d", &totalProcesses);

    for (int i = 0; i < totalProcesses; i++) {
        printf("Enter the ID of Process %d: ", i + 1);
        scanf("%s", processes[i].id);
        printf("Enter the Arrival Time of Process %d: ", i + 1);
        scanf("%d", &processes[i].AT);
        printf("Enter the Burst Time of Process %d: ", i + 1);
        scanf("%d", &processes[i].BT);
        processes[i].remainingBT = processes[i].BT;
        processes[i].RT = -1; 
        processes[i].CT = 0;   
        processes[i].TAT = 0;
        processes[i].WT = 0;
        processes[i].queueLevel = 0; 
        queues[0].arr[queues[0].count++] = &processes[i]; 
    }

    SortProcessesByAT(processes, totalProcesses); 

    MLFQScheduling(queues, 3, processes, totalProcesses);

    DisplayResults(processes, totalProcesses);
    CalculatePerformance(processes, totalProcesses);

    return 0;
}

