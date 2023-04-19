#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Creating 2 Threads
// Function Declaration for implementing later in the program
void *thread_function(void *arg);

int main()
{
    pthread_t thread1, thread2;
    int result1, result2;

    result1 = pthread_create(&thread1, NULL, thread_function, (void *)"Thread 1");
    if (result1 != 0) {
        perror("Thread creation failed");
        return 1;
    }

    result2 = pthread_create(&thread2, NULL, thread_function, (void *)"Thread 2");
    if (result2 != 0) {
        perror("Thread creation failed");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

void *thread_function(void *arg)
{
    char *thread_name = (char *)arg;

    printf("%s is running\n", thread_name);
    sleep(10);

    return NULL;
}
