#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, total = 0, completed = 0;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int req[n], visited[n];
    printf("Enter the disk requests: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &req[i]);
        visited[i] = 0;
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("\nSSTF Disk Scheduling\nOrder: %d", head);
    while (completed < n) {
        int min = 1e9, index = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int diff = abs(req[i] - head);
                if (diff < min) {
                    min = diff;
                    index = i;
                }
            }
        }
        visited[index] = 1;
        total += abs(req[index] - head);
        head = req[index];
        printf(" -> %d", head);
        completed++;
    }

    printf("\nTotal head movement = %d\n", total);
    printf("Average head movement = %.2f\n", (float)total / n);
    return 0;
}
