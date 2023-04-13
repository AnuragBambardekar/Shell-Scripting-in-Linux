#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 8

// Define a global variable to store the number of primes found
int num_primes = 0;

// Define the thread function to calculate prime numbers
void *calc_primes(void *arg) {
    int start = *((int*)arg);
    int end = start + (1000000 / MAX_THREADS);
    int is_prime = 1;
    int i, j;
    pthread_t tid = pthread_self();

    for (i = start; i < end; i++) {
        is_prime = 1;
        for (j = 2; j < i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf("Thread %ld: %d is prime\n", tid, i);
            num_primes++;
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MAX_THREADS];
    int thread_args[MAX_THREADS];
    int i;
    clock_t start_time, end_time;
    double time_taken;

    start_time = clock();

    // Create the worker threads
    for (i = 0; i < MAX_THREADS; i++) {
        thread_args[i] = i * (1000000 / MAX_THREADS);
        pthread_create(&threads[i], NULL, calc_primes, (void*)&thread_args[i]);
    }

    // Wait for the worker threads to finish
    for (i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    end_time = clock();
    time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Found %d primes in %f seconds\n", num_primes, time_taken);

    return 0;
}
