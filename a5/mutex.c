#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int available_tickets; 
pthread_mutex_t mutex; 


typedef struct {
    int thread_id;
    int tickets; 
    int is_booking; 
} ThreadArg;


void* book_tickets(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    int id = data->thread_id;
    int tickets = data->tickets;

    printf("Thread %d started. Booking %d ticket(s)\n", id, tickets);
    
    pthread_mutex_lock(&mutex); // Lock mutex
    if (available_tickets >= tickets) {
        available_tickets -= tickets;
        printf("Thread %d booked %d ticket(s). Remaining: %d\n", id, tickets, available_tickets);
    } else {
        printf("Thread %d failed to book. Only %d tickets available.\n", id, available_tickets);
    }
    pthread_mutex_unlock(&mutex); // Unlock mutex
    
    return NULL;
}

void* cancel_tickets(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    int id = data->thread_id;
    int tickets = data->tickets;

    printf("Thread %d started. Cancelling %d ticket(s)\n", id, tickets);
    
    pthread_mutex_lock(&mutex); // Lock mutex
    available_tickets += tickets;
    printf("Thread %d cancelled %d ticket(s). Total available: %d\n", id, tickets, available_tickets);
    pthread_mutex_unlock(&mutex); // Unlock mutex
    
    return NULL;
}

int main() {
    int num_threads;
    printf("Enter initial number of available tickets: ");
    scanf("%d", &available_tickets);
    printf("Enter number of threads (1-100): ");
    scanf("%d", &num_threads);
    if (num_threads < 1 || num_threads > 100) {
        printf("Number of threads must be between 1 and 100.\n");
        return 1;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[num_threads];
    ThreadArg thread_args[num_threads];
    for (int i = 0; i < num_threads; i++) {
        printf("Enter operation for Thread %d (1 for book, 0 for cancel): ", i + 1);
        scanf("%d", &thread_args[i].is_booking);
        printf("Enter number of tickets for Thread %d: ", i + 1);
        scanf("%d", &thread_args[i].tickets);
        thread_args[i].thread_id = i + 1;
    }
    for (int i = 0; i < num_threads; i++) {
        if (thread_args[i].is_booking) {
            pthread_create(&threads[i], NULL, book_tickets, &thread_args[i]);
        } else {
            pthread_create(&threads[i], NULL, cancel_tickets, &thread_args[i]);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\nFinal tickets available: %d\n", available_tickets);
    pthread_mutex_destroy(&mutex);
    return 0;
}
