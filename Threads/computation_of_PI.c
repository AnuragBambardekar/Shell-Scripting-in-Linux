#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define MAX_THREADS 512

void *compute_pi (void *);

int total_hits, total_misses, hits[MAX_THREADS], sample_points, sample_points_per_thread, num_threads;

//  uses pthreads to compute an approximation of pi using the Monte Carlo method

int main() {
    int i;
    pthread_t p_threads[MAX_THREADS];
    pthread_attr_t attr;

    double computed_pi;
    double time_start, time_end;

    struct timeval tv;
    // struct timezone NULL; //The use of the timezone structure is obsolete; the tz argument should normally be specified as NULL.

    pthread_attr_init (&attr); //  initializes the thread attributes object pointed to by attr with default values.
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM); // sets the contention scope attribute of the thread attributes object pointed to by attr to PTHREAD_SCOPE_SYSTEM, which means that threads are scheduled by the system instead of by the application.
    
    printf("Enter number of sample points: ");
    scanf("%d", &sample_points);

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    gettimeofday(&tv, NULL);
    time_start = (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;

    total_hits = 0;
    sample_points_per_thread = sample_points/num_threads;

    // Create the threads
    for(i=0; i<num_threads; i++)
    {
        hits[i] = i;
        pthread_create(&p_threads[i], &attr, compute_pi, (void *)&hits[i]);
    }

    // After all threads have completed execution, the main function joins each thread with pthread_join and accumulates the hit counts stored in the hits array to compute the total number of hits.
    for(i=0; i<num_threads; i++)
    {
        pthread_join(p_threads[i], NULL);
        printf("%d\n",total_hits);
        total_hits += hits[i];
    }

    // Compute the value of pi
    // The ratio of the number of points inside the circle to the total number of points is used to approximate the value of pi
    computed_pi = 4.0 * (double)total_hits/((double)(sample_points));
    gettimeofday(&tv, NULL);

    time_end = (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;

    printf("Computed PI = %lf\n", computed_pi);
    printf("Time of execution: %lf\n", time_end - time_start);
}

// thread function used to compute the number of hits within a unit circle
void *compute_pi(void *s) {
    int seed, i, *hit_pointer;
    double rand_no_x, rand_no_y;
    int local_hits;

    hit_pointer = (int *)s; // argument s, which is cast to an integer pointer hit_pointer
    seed = *hit_pointer;
    local_hits = 0;

    printf("seed: %d\n",seed);

    for(i=0; i<sample_points_per_thread; i++){
        // Generate 2 random numbers
        // rand() is not threadsafe, but rand_r() is
        // rand_no_x = (double)(rand_r(&seed))/(double)((2<<14)-1);
        // rand_no_y = (double)(rand_r(&seed))/(double)((2<<14)-1);

        // generates a random double value between 0 and 1
        // this is also thread-safe and can be called independently without
        // interfering with the other threads
        rand_no_x = drand48();
        rand_no_y = drand48();

        printf("rand_x: %lf\n",rand_no_x);
        printf("rand_y: %lf\n",rand_no_y);

        // checks if the generated point (rand_no_x, rand_no_y) lies within a unit circle centered at (0.5, 0.5). 
        // If the point lies within the circle, it increments the local variable local_hits.

        printf("%lf\n", ((rand_no_x - 0.5) * (rand_no_x - 0.5) + (rand_no_y - 0.5) * (rand_no_y - 0.5)));
        if(((rand_no_x - 0.5) * (rand_no_x - 0.5) + (rand_no_y - 0.5) * (rand_no_y - 0.5)) < 0.25)
            local_hits++;
        seed *= i;
        printf("local hits: %d\n",total_hits);
    }
    *hit_pointer = local_hits;
    pthread_exit(0);
}