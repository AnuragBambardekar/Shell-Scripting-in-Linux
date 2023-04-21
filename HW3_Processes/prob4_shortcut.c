#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_N 1000000
#define MAX_THREADS 10

bool primes[MAX_N + 1];
int prime_count = 0;
int prime_list[MAX_N];

typedef struct {
    int start;
    int end;
} Range;

void *find_primes(void *arg) {
    Range *range = (Range *)arg;
    int last_prime = 0;
    printf("Thread called!\n");

    // check if there are any previously discovered primes
    if (prime_count > 0) {
        last_prime = prime_list[prime_count - 1];
        if (last_prime > range->start) {
            range->start = last_prime + 1;
        }
    }

    for (int i = range->start; i <= range->end; i++) {
        if (primes[i]) {
            prime_list[prime_count++] = i;
            for (int j = i * i; j <= range->end; j += i) {
                primes[j] = false;
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int num_threads = argc >= 3 ? atoi(argv[2]) : 1;

    if (num_threads < 1 || num_threads > MAX_THREADS) {
        fprintf(stderr, "Invalid number of threads. Must be between 1 and %d\n", MAX_THREADS);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_threads];
    Range ranges[num_threads];

    // initialize the prime array
    for (int i = 2; i <= n; i++) {
        primes[i] = true;
    }

    int block_size = n / num_threads;

    for (int i = 0; i < num_threads; i++) {
        ranges[i].start = i * block_size + 2;
        ranges[i].end = (i == num_threads - 1) ? n : (i + 1) * block_size + 1;

        pthread_create(&threads[i], NULL, find_primes, &ranges[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // print the prime numbers
    printf("Prime numbers up to %d:\n", n);

    for (int i = 0; i < prime_count; i++) {
        printf("%d\n", prime_list[i]);
    }

    return 0;
}
