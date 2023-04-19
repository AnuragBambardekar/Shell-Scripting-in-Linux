#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 10

pthread_t workers[NUM_THREADS];

int main()
{
    for(int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(workers[i], NULL);
    }
    return 0;
}
