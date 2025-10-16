#include <stdio.h>

int n, m;
int alloc[10][10], max[10][10], avail[10];
int need[10][10], finish[10];

void calculateNeed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

int isSafe() {
    int work[10], finish[10] = {0}, safeSeq[10], count = 0;

    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int canRun = 1;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canRun = 0;
                        break;
                    }
                }
                if (canRun) {
                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found)
            return 0; // unsafe
    }

    printf("\nSystem is in SAFE state.\nSafe Sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
    return 1;
}

void resourceRequest() {
    int p;
    int req[10];

    printf("\nEnter process number making request (0-%d): ", n - 1);
    scanf("%d", &p);

    printf("Enter request for %d resources: ", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &req[i]);

    // Step 1: Check if request <= need
    for (int i = 0; i < m; i++) {
        if (req[i] > need[p][i]) {
            printf("\nError: Process has exceeded its maximum claim.\n");
            return;
        }
    }

    // Step 2: Check if request <= available
    for (int i = 0; i < m; i++) {
        if (req[i] > avail[i]) {
            printf("\nResources not available. Process must wait.\n");
            return;
        }
    }

    // Step 3: Pretend to allocate
    for (int i = 0; i < m; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }

    // Step 4: Check safety
    if (isSafe())
        printf("\nRequest can be granted safely.\n");
    else {
        printf("\nRequest cannot be granted (unsafe state). Rolling back...\n");
        // Rollback
        for (int i = 0; i < m; i++) {
            avail[i] += req[i];
            alloc[p][i] -= req[i];
            need[p][i] += req[i];
        }
    }
}

int main() {
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

    calculateNeed();

    printf("\nInitial Safety Check:\n");
    if (!isSafe()) {
        printf("\nInitial system state is unsafe. Exiting...\n");
        return 0;
    }

    // Process resource request
    resourceRequest();

    return 0;
}
