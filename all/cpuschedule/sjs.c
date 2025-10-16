#include <stdio.h>

int main() {
    int n, bt[20], p[20], wt[20], tat[20], i, j, temp;
    float avgwt = 0, avgtat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst times:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
        p[i] = i + 1;
    }

    // Sort by burst time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (bt[j] > bt[j + 1]) {
                temp = bt[j]; bt[j] = bt[j + 1]; bt[j + 1] = temp;
                temp = p[j]; p[j] = p[j + 1]; p[j + 1] = temp;
            }
        }
    }

    wt[0] = 0;
    for (i = 1; i < n; i++) {
        wt[i] = 0;
        for (j = 0; j < i; j++)
            wt[i] += bt[j];
        avgwt += wt[i];
    }

    printf("\nProcess\tBT\tWT\tTAT");
    for (i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
        avgtat += tat[i];
        printf("\nP%d\t%d\t%d\t%d", p[i], bt[i], wt[i], tat[i]);
    }

    printf("\n\nAverage Waiting Time = %.2f", avgwt / n);
    printf("\nAverage Turnaround Time = %.2f", avgtat / n);

    return 0;
}
