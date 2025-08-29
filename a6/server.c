#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define MAX_MSG 256

typedef struct {
    char client_msg[MAX_MSG];
    char server_msg[MAX_MSG];
    int client_ready;
    int server_ready;
    int chat_active;
} ChatData;

int main() {
    key_t key;
    int shmid;
    ChatData *chat_data;
    char message[MAX_MSG];
    
    // Generate a key for shared memory
    key = ftok("/tmp", 66);
    
    // Create shared memory segment
    shmid = shmget(key, sizeof(ChatData), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Server: shmget failed");
        exit(1);
    }
    
    // Attach shared memory
    chat_data = (ChatData *)shmat(shmid, NULL, 0);
    if (chat_data == (ChatData *)-1) {
        perror("Server: shmat failed");
        exit(1);
    }
    
    // Initialize shared data
    memset(chat_data, 0, sizeof(ChatData));
    chat_data->chat_active = 1;
    
    printf("Server: Chat server started. Waiting for client...\n");
    
    while (chat_data->chat_active) {
        // Wait for client message
        while (!chat_data->client_ready && chat_data->chat_active) {
            sleep(1);
        }
        
        if (!chat_data->chat_active) break;
        
        printf("Received from client: %s\n", chat_data->client_msg);
        
        // Check if client wants to exit
        if (strcmp(chat_data->client_msg, "bye") == 0 || 
            strcmp(chat_data->client_msg, "Bye") == 0) {
            printf("Server: Client disconnected.\n");
            chat_data->chat_active = 0;
            break;
        }
        
        // Reset client ready flag
        chat_data->client_ready = 0;
        
        // Get server response
        printf("Enter response: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline
        
        // Copy to shared memory
        strcpy(chat_data->server_msg, message);
        chat_data->server_ready = 1;
        
        // Check if server wants to exit
        if (strcmp(message, "bye") == 0 || strcmp(message, "Bye") == 0) {
            printf("Server: Shutting down...\n");
            chat_data->chat_active = 0;
            break;
        }
    }
    
    // Cleanup
    shmdt(chat_data);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Server: Shared memory cleaned up.\n");
    
    return 0;
}