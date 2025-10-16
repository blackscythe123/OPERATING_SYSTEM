#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, total = 0;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int req[n];
    printf("Enter the disk requests: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    int temp, size = n + 1;
    int arr[size];
    for (int i = 0; i < n; i++)
        arr[i] = req[i];
    arr[n] = head;

    // Sort
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

    printf("\nC-SCAN Disk Scheduling\nOrder: %d", head);

    // Move right
    for (int i = pos + 1; i < size; i++) {
        total += abs(arr[i] - head);
        head = arr[i];
        printf(" -> %d", head);
    }

    // Jump from end to start
    total += (199 - head); // move to max
    total += 199;          // wrap to start (0)
    head = 0;

    // Continue servicing from start
    for (int i = 0; i < pos; i++) {
        total += abs(arr[i] - head);
        head = arr[i];
        printf(" -> %d", head);
    }

    printf("\nTotal head movement = %d\n", total);
    printf("Average head movement = %.2f\n", (float)total / n);
    return 0;
}
