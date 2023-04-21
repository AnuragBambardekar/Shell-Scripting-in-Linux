#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_N 1000000
#define MAX_THREADS 10

bool primes[MAX_N + 1];
int prime_count = 0;
int prime_list[MAX_N];
int double_prime_count = 0;
int double_prime_list[MAX_N];

typedef struct {
    int start;
    int end;
} Range;

typedef struct {
    int *primes;
    int count;
    int start;
    int end;
} DoublePrimeRange;

void *find_primes(void *arg) {
    Range *range = (Range *)arg;
    int last_prime = 0;

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

void *find_double_primes(void *arg) {
    DoublePrimeRange *range = (DoublePrimeRange *)arg;

    for (int i = range->start; i <= range->end; i++) {
        int n = range->primes[i];
        int m = 0;

        // reverse the digits of n
        while (n > 0) {
            m = m * 10 + n % 10;
            n /= 10;
        }

        if (m == range->primes[i]) {
            continue;
        }

        // check if m is also prime
        bool is_prime = true;
        for (int j = 2; j * j <= m; j++) {
            if (m % j == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            double_prime_list[double_prime_count++] = range->primes[i];
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
    int num_double_prime_threads = argc >= 4 ? atoi(argv[3]) : 1;

    if (num_threads < 1 || num_threads > MAX_THREADS) {
        fprintf(stderr, "Invalid number of threads. Must be between 1 and %d\n", MAX_THREADS);
        exit(EXIT_FAILURE);
    }

    if (num_double_prime_threads < 1 || num_double_prime_threads > MAX_THREADS) {
        fprintf(stderr, "Invalid number of double prime threads. Must be between 1 and %d\n", MAX_THREADS);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_threads];
    Range ranges[num_threads];

    pthread_t double_prime_threads[num_double_prime_threads];
    DoublePrimeRange double_prime_ranges[num_double_prime_threads];

    // initialize the prime array
    for (int i = 2; i <= n; i++) {
        primes[i] = true;
    }

    // determine the range for each thread
    int range_size = (n - 1) / num_threads + 1;
    for (int i = 0; i < num_threads; i++) {
        ranges[i].start = i * range_size + 2;
        ranges[i].end = (i + 1) * range_size + 1;
        if (ranges[i].end > n) {
            ranges[i].end = n;
        }
    }
   
    // create threads to find primes
    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_create(&threads[i], NULL, find_primes, &ranges[i]);
        if (rc != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // determine the range for each double prime thread
    int double_prime_range_size = prime_count / num_double_prime_threads + 1;
    for (int i = 0; i < num_double_prime_threads; i++) {
        double_prime_ranges[i].primes = prime_list;
        double_prime_ranges[i].count = prime_count;
        double_prime_ranges[i].start = i * double_prime_range_size;
        double_prime_ranges[i].end = (i + 1) * double_prime_range_size - 1;
        if (double_prime_ranges[i].end >= prime_count) {
            double_prime_ranges[i].end = prime_count - 1;
        }
    }

    // create threads to find double primes
    for (int i = 0; i < num_double_prime_threads; i++) {
        int rc = pthread_create(&double_prime_threads[i], NULL, find_double_primes, &double_prime_ranges[i]);
        if (rc != 0) {
            fprintf(stderr, "Error creating double prime thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // wait for double prime threads to finish
    for (int i = 0; i < num_double_prime_threads; i++) {
        int rc = pthread_join(double_prime_threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "Error joining double prime thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // print out the double prime numbers
    if(double_prime_count > 0){
        for (int i = 0; i < double_prime_count; i++) {
            printf("%d\n", double_prime_list[i]);
        }
    } else {
        printf("No double-primes found in the given range.\n");
    }
    

    return 0;
}