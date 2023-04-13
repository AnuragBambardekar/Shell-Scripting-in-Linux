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
    pthread_t thread;
    int result, i;
    clock_t start, end;
    double cpu_time_used;

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

    // Create a thread to mark the prime numbers
    result = pthread_create(&thread, NULL, mark_primes, NULL);
    if (result != 0) {
        fprintf(stderr, "Error: Thread creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    // Stop the clock
    end = clock();

    // Print out the prime numbers
    for (i = 2; i <= MAX_NUM; i++) {
        if (is_prime[i]) {
            printf("%d is prime\n", i);
            num_primes++;
            // sleep(1);
        }
    }

    // Free the memory used by the is_prime array
    free(is_prime);

    // Calculate the time taken for execution
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);
    printf("Found %d primes in %f seconds\n", num_primes, cpu_time_used);

    return 0;
}

void *mark_primes(void *arg)
{
    int i, j;

    // Mark non-prime numbers using the Sieve of Eratosthenes algorithm
    for (i = 2; i * i <= MAX_NUM; i++) {
        if (is_prime[i]) {
            for (j = i * i; j <= MAX_NUM; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    pthread_exit(NULL);
}
