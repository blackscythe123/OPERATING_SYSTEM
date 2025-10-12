#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10
#define MAX_NAME 20

typedef struct
{
    char name[MAX_NAME];
    int size;
    int start_block;
    int allocation_type; // 0: sequential, 1: linked
    int permissions;     // bitmask: 1=read, 2=write, 4=execute
} File;

int disk_size;
int *disk;
int *next_block;
File files[MAX_FILES];
int file_count = 0;

void init_disk();
int find_free_blocks(int num, int *blocks);
int allocate_sequential(int size, int *start);
int allocate_linked(int size, int *start);
int create_file(char *name, int size, int type);
void delete_file(char *name);
void access_file(char *name);
void display_permissions(File *f);
void change_permissions(char *name, int perm);

void init_disk()
{
    disk = (int *)malloc(disk_size * sizeof(int));
    next_block = (int *)malloc(disk_size * sizeof(int));
    for (int i = 0; i < disk_size; i++)
    {
        disk[i] = -1; // free
        next_block[i] = -1;
    }
}

int find_free_blocks(int num, int *blocks)
{
    int count = 0;
    for (int i = 0; i < disk_size && count < num; i++)
    {
        if (disk[i] == -1)
        {
            blocks[count++] = i;
        }
    }
    return count == num;
}

int allocate_sequential(int size, int *start)
{
    for (int i = 0; i <= disk_size - size; i++)
    {
        int contiguous = 1;
        for (int j = 0; j < size; j++)
        {
            if (disk[i + j] != -1)
            {
                contiguous = 0;
                break;
            }
        }
        if (contiguous)
        {
            *start = i;
            for (int j = 0; j < size; j++)
            {
                disk[i + j] = file_count;
            }
            return 1;
        }
    }
    return 0;
}

int allocate_linked(int size, int *start)
{
    int blocks[size];
    if (!find_free_blocks(size, blocks))
        return 0;
    *start = blocks[0];
    for (int i = 0; i < size; i++)
    {
        disk[blocks[i]] = file_count;
        if (i < size - 1)
        {
            next_block[blocks[i]] = blocks[i + 1];
        }
        else
        {
            next_block[blocks[i]] = -1;
        }
    }
    return 1;
}

int create_file(char *name, int size, int type)
{
    if (file_count >= MAX_FILES)
        return 0;
    int start;
    int success = 0;
    if (type == 0)
    { // sequential
        success = allocate_sequential(size, &start);
    }
    else
    { // linked
        success = allocate_linked(size, &start);
    }
    if (success)
    {
        strcpy(files[file_count].name, name);
        files[file_count].size = size;
        files[file_count].start_block = start;
        files[file_count].allocation_type = type;
        files[file_count].permissions = 7; // default rwx
        file_count++;
        printf("File '%s' allocated at blocks: ", name);
        int current = start;
        for (int i = 0; i < size; i++)
        {
            printf("%d", current);
            if (i < size - 1)
                printf(", ");
            if (type == 1)
                current = next_block[current];
            else
                current++;
        }
        printf(" (%s Allocation)\n", type == 0 ? "Sequential" : "Linked");
    }
    else
    {
        printf("File '%s' cannot be allocated (insufficient %s blocks)\n", name, type == 0 ? "contiguous" : "");
    }
    return success;
}

void delete_file(char *name)
{
    int idx = -1;
    for (int i = 0; i < file_count; i++)
    {
        if (strcmp(files[i].name, name) == 0)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("File '%s' not found\n", name);
        return;
    }
    // Collect blocks first
    int blocks[files[idx].size];
    int current = files[idx].start_block;
    for (int i = 0; i < files[idx].size; i++)
    {
        blocks[i] = current;
        if (files[idx].allocation_type == 1)
            current = next_block[current];
        else
            current++;
    }
    // Free the blocks
    for (int i = 0; i < files[idx].size; i++)
    {
        disk[blocks[i]] = -1;
        next_block[blocks[i]] = -1;
    }
    // shift files
    for (int i = idx; i < file_count - 1; i++)
    {
        files[i] = files[i + 1];
    }
    file_count--;
    printf("File '%s' deleted\n", name);
}

void access_file(char *name)
{
    int idx = -1;
    for (int i = 0; i < file_count; i++)
    {
        if (strcmp(files[i].name, name) == 0)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("File '%s' not found\n", name);
        return;
    }
    if (!(files[idx].permissions & 1))
    {
        printf("Access denied: no read permission\n");
        return;
    }
    printf("File '%s' allocated at blocks: ", name);
    int current = files[idx].start_block;
    for (int i = 0; i < files[idx].size; i++)
    {
        printf("%d", current);
        if (i < files[idx].size - 1)
            printf(", ");
        if (files[idx].allocation_type == 1)
            current = next_block[current];
        else
            current++;
    }
    printf(" (%s Allocation)\n", files[idx].allocation_type == 0 ? "Sequential" : "Linked");
    display_permissions(&files[idx]);
}

void display_permissions(File *f)
{
    printf("Permissions: ");
    if (f->permissions & 1)
        printf("r");
    else
        printf("-");
    if (f->permissions & 2)
        printf("w");
    else
        printf("-");
    if (f->permissions & 4)
        printf("x");
    else
        printf("-");
    printf("\n");
}

void change_permissions(char *name, int perm)
{
    int idx = -1;
    for (int i = 0; i < file_count; i++)
    {
        if (strcmp(files[i].name, name) == 0)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("File '%s' not found\n", name);
        return;
    }
    files[idx].permissions = perm;
    printf("Permissions for '%s' changed to: ", name);
    display_permissions(&files[idx]);
}

int main()
{
    printf("Enter disk size: ");
    scanf("%d", &disk_size);
    init_disk();
    int num_files;
    printf("Enter number of files: ");
    scanf("%d", &num_files);
    for (int i = 0; i < num_files; i++)
    {
        char name[MAX_NAME];
        int size, type;
        printf("File %d:\n", i + 1);
        printf("File name: ");
        scanf("%s", name);
        printf("File size: ");
        scanf("%d", &size);
        printf("Allocation Technique (0: Sequential, 1: Linked): ");
        scanf("%d", &type);
        create_file(name, size, type);
    }
    // Operations loop
    while (1)
    {
        printf("\nOperations:\n1. Create File\n2. Delete File\n3. Access File\n4. Change Permissions\n5. Exit\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            char name[MAX_NAME];
            int size, type;
            printf("File name: ");
            scanf("%s", name);
            printf("File size: ");
            scanf("%d", &size);
            printf("Allocation Technique (0: Sequential, 1: Linked): ");
            scanf("%d", &type);
            create_file(name, size, type);
        }
        else if (choice == 2)
        {
            char name[MAX_NAME];
            printf("File name to delete: ");
            scanf("%s", name);
            delete_file(name);
        }
        else if (choice == 3)
        {
            char name[MAX_NAME];
            printf("File name to access: ");
            scanf("%s", name);
            access_file(name);
        }
        else if (choice == 4)
        {
            char name[MAX_NAME];
            int perm;
            printf("File name: ");
            scanf("%s", name);
            printf("New permissions (bitmask: 1=r, 2=w, 4=x, e.g. 7=rwx): ");
            scanf("%d", &perm);
            change_permissions(name, perm);
        }
        else if (choice == 5)
        {
            break;
        }
    }
    free(disk);
    free(next_block);
    return 0;
}
