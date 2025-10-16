#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, total = 0, direction;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int req[n];
    printf("Enter the disk requests: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter head movement direction (1=Right, 0=Left): ");
    scanf("%d", &direction);

    int temp, size = n + 1;
    int arr[size];
    for (int i = 0; i < n; i++)
        arr[i] = req[i];
    arr[n] = head;

    // Sort all requests including head
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }

    int pos;
    for (int i = 0; i < size; i++)
        if (arr[i] == head)
            pos = i;

    printf("\nSCAN Disk Scheduling\nOrder: %d", head);

    if (direction == 1) {
        // Move right
        for (int i = pos + 1; i < size; i++) {
            total += abs(arr[i] - head);
            head = arr[i];
            printf(" -> %d", head);
        }
        // Move to end (199 assumed as max track)
        total += abs(199 - head);
        head = 199;
        // Move left
        for (int i = pos - 1; i >= 0; i--) {
            total += abs(arr[i] - head);
            head = arr[i];
            printf(" -> %d", head);
        }
    } else {
        // Move left
        for (int i = pos - 1; i >= 0; i--) {
            total += abs(arr[i] - head);
            head = arr[i];
            printf(" -> %d", head);
        }
        // Move to start (0)
        total += abs(head - 0);
        head = 0;
        // Move right
        for (int i = pos + 1; i < size; i++) {
            total += abs(arr[i] - head);
            head = arr[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal head movement = %d\n", total);
    printf("Average head movement = %.2f\n", (float)total / n);
    return 0;
}
