#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void recursion(int n);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t thread;
    if (pthread_create(&thread, NULL, (void* )recursion, (void* )5) != 0)
    {
        printf("Error with creating thread.\n");
        return 1;
    }
    else
    {
        printf("Thread was created.\n");
    }
    if (pthread_join(thread, NULL) != 0)
    {
        printf("Error with joining thread.\n");
        return 1;
    }
    return 0;
}

void recursion(int n)
{
    printf("Before lock.\n");
    pthread_mutex_lock(&mutex);
    if (n > 1)
    {
        printf("n = %d\n", n);
        n--;
        recursion(n);
    }
    else
    {
        printf("Done!");
    }
    pthread_mutex_unlock(&mutex);
}