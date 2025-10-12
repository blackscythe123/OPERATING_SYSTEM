// C program for Banker's Algorithm with Menu-driven Interface
#include <stdio.h>

#define MAX_P 10
#define MAX_R 10

int main() {
    int n = 0, r = 0;
    int avail[MAX_R], alloc[MAX_P][MAX_R], max[MAX_P][MAX_R], need[MAX_P][MAX_R];
    char res_names[MAX_R] = {'A','B','C','D','E','F','G','H','I','J'};
    int data_read = 0; // Flag to check if data has been read

    while (1) {
        printf("\nBanker's Algorithm\n");
        printf("1. Read Data\n");
        printf("2. Print Data\n");
        printf("3. Safety Sequence\n");
        printf("4. Exit\n");
        printf("Enter the option: ");
        int option;
        scanf("%d", &option);

        if (option == 1) {
            // Read Data
            printf("Number of processes: ");
            scanf("%d", &n);
            printf("P0");
            for (int i = 1; i < n; i++) printf(", P%d", i);
            printf("\nNumber of resources: ");
            scanf("%d", &r);
            for (int i = 0; i < r; i++) {
                printf("Number of Available instances of %c: ", res_names[i]);
                scanf("%d", &avail[i]);
            }
            for (int i = 0; i < n; i++) {
                printf("Maximum requirement for P%d: ", i);
                for (int j = 0; j < r; j++) {
                    scanf("%d", &max[i][j]);
                }
            }
            for (int i = 0; i < n; i++) {
                printf("Allocated instances to P%d: ", i);
                for (int j = 0; j < r; j++) {
                    scanf("%d", &alloc[i][j]);
                }
            }
            // Calculate Need matrix
            for (int i = 0; i < n; i++)
                for (int j = 0; j < r; j++)
                    need[i][j] = max[i][j] - alloc[i][j];
            data_read = 1;
        } else if (option == 2) {
            if (!data_read) {
                printf("Please read data first (Option 1).\n");
                continue;
            }
            // Print Data
            printf("Pid\tAlloc\tMax\tNeed\tAvail\n");
            printf("\t");
            for (int j = 0; j < r; j++) printf("%c ", res_names[j]);
            printf("\t");
            for (int j = 0; j < r; j++) printf("%c ", res_names[j]);
            printf("\t");
            for (int j = 0; j < r; j++) printf("%c ", res_names[j]);
            printf("\t");
            for (int j = 0; j < r; j++) printf("%c ", res_names[j]);
            printf("\n");

            for (int i = 0; i < n; i++) {
                printf("P%d\t", i);
                for (int j = 0; j < r; j++) printf("%d ", alloc[i][j]);
                printf("\t");
                for (int j = 0; j < r; j++) printf("%d ", max[i][j]);
                printf("\t");
                for (int j = 0; j < r; j++) printf("%d ", need[i][j]);
                printf("\t");
                if (i == 0) // Show Avail only for first row
                    for (int j = 0; j < r; j++) printf("%d ", avail[j]);
                printf("\n");
            }
        } else if (option == 3) {
            if (!data_read) {
                printf("Please read data first (Option 1).\n");
                continue;
            }
            // Safety Sequence
            int work[MAX_R], finish[MAX_P], safe_seq[MAX_P], idx = 0;
            for (int i = 0; i < r; i++) work[i] = avail[i];
            for (int i = 0; i < n; i++) finish[i] = 0;

            // Find a safe sequence
            for (int count = 0; count < n; count++) {
                int found = 0;
                for (int p = 0; p < n; p++) {
                    if (!finish[p]) {
                        int can_allocate = 1;
                        for (int j = 0; j < r; j++) {
                            if (need[p][j] > work[j]) {
                                can_allocate = 0;
                                break;
                            }
                        }
                        if (can_allocate) {
                            for (int j = 0; j < r; j++)
                                work[j] += alloc[p][j];
                            safe_seq[idx++] = p;
                            finish[p] = 1;
                            found = 1;
                        }
                    }
                }
                if (!found) break;
            }

            printf("Display the Safety Sequence: ");
            if (idx == n) {
                for (int i = 0; i < n; i++)
                    printf("P%d ", safe_seq[i]);
                printf("\n");
            } else {
                printf("No safe sequence exists (system is in unsafe state).\n");
            }
        } else if (option == 4) {
            // Exit
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid option! Try again.\n");
        }
    }
    return 0;
}