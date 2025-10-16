#include <stdio.h>

int main() {
    int n, m; // n = processes, m = resources
    int alloc[10][10], max[10][10], avail[10];
    int need[10][10], finish[10], safeSeq[10];
    int count = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("\nEnter Max Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("\nEnter Available Resources:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    // Calculate Need matrix = Max - Allocation
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Initialize all processes as unfinished
    for (int i = 0; i < n; i++)
        finish[i] = 0;

    printf("\nNeed Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    int safeIndex = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int canRun = 1;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        canRun = 0;
                        break;
                    }
                }
                if (canRun) {
                    for (int k = 0; k < m; k++)
                        avail[k] += alloc[i][k];

                    safeSeq[safeIndex++] = i;
                    finish[i] = 1;
                    count++;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("\nSystem is in UNSAFE state!\n");
            return 0;
        }
    }

    printf("\nSystem is in SAFE state.\nSafe Sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}
