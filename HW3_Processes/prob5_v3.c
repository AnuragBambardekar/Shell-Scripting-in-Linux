#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
# include <bits/sigaction.h> // for sigaction handler
#ifndef __sigset_t_defined
#define __sigset_t_defined 1

#include <bits/types/__sigset_t.h>

/* A set of signals to be blocked, unblocked, or waited for.  */
typedef __sigset_t sigset_t;
#endif

#define NUM_THREADS 3
#define NUM_ITERATIONS 1000

// global variables to hold the signal number and the sums
int signal_num[NUM_THREADS];
long long int sum[NUM_THREADS];

// global variable to hold the threads
pthread_t threads[NUM_THREADS];

// signal handlers for each thread
void sigfpe_handler(int sig)
{
    signal_num[0] = sig;
    printf("Thread 0: Signal received: %d\n", sig);
}

void sigstop_handler(int sig)
{
    signal_num[1] = sig;
    printf("Thread 1: Signal received: %d\n", sig);
}

void sigsegv_handler(int sig)
{
    signal_num[2] = sig;
    printf("Thread 2: Signal received: %d\n", sig);
}

// function to be executed by each thread
void *thread_func(void *arg)
{
    int tid = *((int*)arg);
    // set up the signal handlers for this thread
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    if(tid == 0) {
        sa.sa_handler = sigfpe_handler;
        sigaction(SIGFPE, &sa, NULL);
        sigfillset(&sa.sa_mask);
        sigdelset(&sa.sa_mask, SIGFPE);
        sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL);
    } else if(tid == 1) {
        sa.sa_handler = sigstop_handler;
        sigaction(SIGPIPE, &sa, NULL);
        sigfillset(&sa.sa_mask);
        sigdelset(&sa.sa_mask, SIGPIPE);
        sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL);
    } else if(tid == 2) {
        sa.sa_handler = sigsegv_handler;
        sigaction(SIGSEGV, &sa, NULL);
        sigfillset(&sa.sa_mask);
        sigdelset(&sa.sa_mask, SIGSEGV);
        sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL);
    }
    // compute the sum of integers from 0 to 1,000 x tid
    for(int i = 0; i <= NUM_ITERATIONS; i++) {
        sum[tid] += i + NUM_ITERATIONS * tid;
    }
    printf("Thread %d: Sum = %lld\n", tid, sum[tid]);

    sleep(5); // let the threads run
    // send a SIGPIPE signal to thread 2
    pthread_kill(threads[1], SIGPIPE);
    return NULL;
}

int main()
{
    int thread_ids[NUM_THREADS];
    // create the threads
    for(int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
        
    }
    // wait for all threads to finish
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
