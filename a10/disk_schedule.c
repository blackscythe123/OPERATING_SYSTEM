#include <stdio.h>
#include <stdlib.h>

int getRequest(int requests[], int n, int head)
{
    int minDist = 100000, index = -1;
    for (int i = 0; i < n; i++)
    {
        if (requests[i] != -1)
        {
            int dist = abs(requests[i] - head);
            if (dist < minDist)
            {
                minDist = dist;
                index = i;
            }
        }
    }
    return index;
}

void sstf(int requests[], int n, int head)
{
    int totalSeek = 0, currHead = head, served = 0;
    while (served < n)
    {
        int index = getRequest(requests, n, currHead);
        if (index == -1)
            break;
        totalSeek += abs(requests[index] - currHead);
        currHead = requests[index];
        requests[index] = -1;
        served++;
    }
    printf("Total Seek Time = %d\n", totalSeek);
}

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void cscan(int requests[], int n, int head, int disk_size)
{
    int totalSeek = 0, currHead = head;
    qsort(requests, n, sizeof(int), cmpfunc);
    int i;
    for (i = 0; i < n; i++)
    {
        if (requests[i] >= head)
            break;
    }
    for (int j = i; j < n; j++)
    {
        totalSeek += abs(requests[j] - currHead);
        currHead = requests[j];
    }
    if (i > 0)
    {
        totalSeek += abs(disk_size - 1 - currHead);
        totalSeek += disk_size - 1;
        currHead = 0;
        for (int j = 0; j < i; j++)
        {
            totalSeek += abs(requests[j] - currHead);
            currHead = requests[j];
        }
    }
    printf("Total Seek Time = %d\n", totalSeek);
}

int main()
{
    int n, head, disk_size, choice;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    int requests[n], copyRequests[n];
    printf("Enter the requests: ");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &requests[i]);
        copyRequests[i] = requests[i];
    }
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size: ");
    scanf("%d", &disk_size);

    do
    {
        printf("\n********** MENU **********\n");
        printf("1. SSTF\n2. C-SCAN\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            sstf(copyRequests, n, head);
            break;
        case 2:
            for (int i = 0; i < n; i++)
                copyRequests[i] = requests[i];
            cscan(copyRequests, n, head, disk_size);
            break;
        case 3:
            printf("Exit\n");
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 3);
    return 0;
}
