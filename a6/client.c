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
    
    // Generate the same key as server
    key = ftok("/tmp", 66);
    
    // Get existing shared memory segment
    shmid = shmget(key, sizeof(ChatData), 0666);
    if (shmid == -1) {
        perror("Client: shmget failed - make sure server is running first");
        exit(1);
    }
    
    // Attach shared memory
    chat_data = (ChatData *)shmat(shmid, NULL, 0);
    if (chat_data == (ChatData *)-1) {
        perror("Client: shmat failed");
        exit(1);
    }
    
    printf("Client: Connected to chat server\n");
    printf("Client: Type 'bye' to exit\n");
    
    while (chat_data->chat_active) {
        // Get client message
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline
        
        // Copy to shared memory
        strcpy(chat_data->client_msg, message);
        chat_data->client_ready = 1;
        
        // Check if client wants to exit
        if (strcmp(message, "bye") == 0 || strcmp(message, "Bye") == 0) {
            printf("Client: Disconnecting...\n");
            break;
        }
        
        // Wait for server response
        while (!chat_data->server_ready && chat_data->chat_active) {
            sleep(1);
        }
        
        if (!chat_data->chat_active) break;
        
        printf("Received from server: %s\n", chat_data->server_msg);
        
        // Check if server wants to exit
        if (strcmp(chat_data->server_msg, "bye") == 0 || 
            strcmp(chat_data->server_msg, "Bye") == 0) {
            printf("Client: Server disconnected.\n");
            break;
        }
        
        // Reset server ready flag
        chat_data->server_ready = 0;
    }
    
    // Cleanup
    shmdt(chat_data);
    printf("Client: Disconnected from server.\n");
    
    return 0;
}