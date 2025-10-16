#include <stdio.h>

int findLRU(int time[], int n) {
    int min = time[0], pos = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

int main() {
    int n, f, i, j, pos, page_faults = 0, counter = 0;
    int pages[30], frames[10], time[10];

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter page reference string: ");
    for (i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &f);

    for (i = 0; i < f; i++)
        frames[i] = -1;

    printf("\nPage\tFrames\n");
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                time[j] = ++counter;
                break;
            }
        }

        if (!found) {
            int empty = -1;
            for (j = 0; j < f; j++) {
                if (frames[j] == -1) {
                    empty = j;
                    break;
                }
            }

            if (empty != -1) {
                frames[empty] = pages[i];
                time[empty] = ++counter;
            } else {
                pos = findLRU(time, f);
                frames[pos] = pages[i];
                time[pos] = ++counter;
            }
            page_faults++;
        }

        printf("%d\t", pages[i]);
        for (j = 0; j < f; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\n");
    }

    printf("\nTotal Page Faults = %d\n", page_faults);
    return 0;
}
