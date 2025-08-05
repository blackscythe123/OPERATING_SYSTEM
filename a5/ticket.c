#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//creating a global variable so resources can be used by all threads 
int ticket_count = 100;
pthread_mutex_t mutex;

int create_thread(pthread_t thread,void *(*function)(void *), void *arg)
{
    return pthread_create(thread,NULL,function,arg);
}

int join_thread(pthread_t __th, void **__thread_return)
{
    return pthread_join(__th,__thread_return);
}

int mutex_lock(pthread_mutex_t *__mutex)
{
    pthread_mutex_lock(__mutex);
}

int mutex_unlock(pthread_mutex_t *__mutex)
{
    pthread_mutex_unlock(__mutex);
}
