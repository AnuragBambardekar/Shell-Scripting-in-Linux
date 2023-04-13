#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_NUM 1000000

void *mark_primes(void *arg);

char *is_prime;
int num_primes = 0;

int main()
{
    pthread_t threads[8];
    int i, result;
    clock_t start, end;
    double cpu_time_used;
    pthread_t tid = pthread_self();

    // Allocate memory for the is_prime array
    is_prime = (char *)malloc(MAX_NUM + 1);
    if (is_prime == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the is_prime array
    for (i = 2; i <= MAX_NUM; i++) {
        is_prime[i] = 1;
    }

    // Start the clock
    start = clock();

    // Create eight threads to mark the prime numbers
    for (i = 0; i < 8; i++) { // CREATED n Threads
        result = pthread_create(&threads[i], NULL, mark_primes, (void *)(long)i);
        printf("Thread %d ,TID: %ld \n", i, tid);
        // sleep(1); // Can uncomment this to see above statement being printed
        if (result != 0) {
            fprintf(stderr, "Error: Thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for the threads to finish
    for (i = 0; i < 8; i++) { // Wait for n Threads
        pthread_join(threads[i], NULL);
    }

    // Stop the clock
    end = clock();

    // Print out the prime numbers and count them
    for (i = 2; i <= MAX_NUM; i++) {
        if (is_prime[i]) {
            printf("%d is prime\n", i);
            num_primes++;
        }
    }
    printf("Found %d primes\n", num_primes);

    // Free the memory used by the is_prime array
    free(is_prime);

    // Calculate the time taken for execution
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);

    return 0;
}

void *mark_primes(void *arg)
{
    int i, j, start, end;
    long thread_id = (long)arg;
    start = (MAX_NUM / 8) * thread_id + 2;
    end = (MAX_NUM / 8) * (thread_id + 1);

    // Mark non-prime numbers using the Sieve of Eratosthenes algorithm
    for (i = 2; i * i <= end; i++) {
        if (is_prime[i]) {
            for (j = i * i; j <= end; j += i) {
                if (j >= start) {
                    is_prime[j] = 0;
                }
            }
        }
    }

    pthread_exit(NULL);
}
