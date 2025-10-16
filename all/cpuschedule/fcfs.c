#include <stdio.h>

int main() {
    int n, bt[20], wt[20], tat[20];
    float avgwt = 0, avgtat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    wt[0] = 0;
    for (int i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    for (int i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
        avgwt += wt[i];
        avgtat += tat[i];
    }

    printf("\nProcess\tBT\tWT\tTAT");
    for (int i = 0; i < n; i++)
        printf("\nP%d\t%d\t%d\t%d", i + 1, bt[i], wt[i], tat[i]);

    printf("\n\nAverage Waiting Time = %.2f", avgwt / n);
    printf("\nAverage Turnaround Time = %.2f", avgtat / n);

    return 0;
}
