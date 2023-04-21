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

pthread_t tid[NUM_THREADS];

void *thread_func(void *arg);
void signal_handler(int signo);

void sigusr1_handler(int sig) {
    printf("USR1 invoked!\n");
    // exit(0);
}

int main() {
    int i, ret;
    sigset_t set;

    struct sigaction act;
    act.sa_handler = sigusr1_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);

    // Block signals until threads are created
    sigemptyset(&set);
    sigaddset(&set, SIGFPE);
    sigaddset(&set, SIGPIPE);
    // sigaddset(&set, SIGSTOP); // cannot be caught
    sigaddset(&set, SIGSEGV);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    int thread_args[NUM_THREADS];

    // Print waiting message before thread creation
    printf("Waiting to receive signal from terminal before thread creation...\n");
    // sleep(25);
    sleep(5);


    // Create threads
    for (i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i + 1;
        ret = pthread_create(&tid[i], NULL, thread_func, &thread_args[i]);
        if (ret != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }

    // Print waiting message after thread creation
    printf("Waiting to receive signal from terminal after thread creation...\n");
    // sleep(25);

    // Wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        ret = pthread_join(tid[i], NULL);
        if (ret != 0) {
            fprintf(stderr, "pthread_join failed: %d\n", ret);
            exit(EXIT_FAILURE);
        }
        printf("Thread %d joined\n",i+1);
    }

    
    // Restore default signal handling
    // signal(SIGFPE, SIG_DFL);
    // signal(SIGPIPE, SIG_DFL);
    // // signal(SIGSTOP, SIG_DFL); // cannot be caught
    // signal(SIGSEGV, SIG_DFL);

    // Restore default signal handling and unblock signals
    struct sigaction def_act;
    def_act.sa_handler = SIG_DFL;
    sigemptyset(&def_act.sa_mask);
    def_act.sa_flags = 0;
    sigaction(SIGFPE, &def_act, NULL);
    sigaction(SIGPIPE, &def_act, NULL);
    sigaction(SIGSEGV, &def_act, NULL);
    sigemptyset(&set);
    pthread_sigmask(SIG_SETMASK, &set, NULL);

    printf("Waiting to receive signal from terminal after threads are joined...\n");
    // fflush(stdout);
    sleep(25);

    return 0;
}

void *thread_func(void *arg) {
    int i, sum = 0;
    struct sigaction act;
    sigset_t set;
    int tid = *(int*)arg;

    switch(tid) {
        case 1:
            // catch SIGFPE and block SIGPIPE and SIGSEGV
            sigaddset(&set, SIGFPE);
            sigprocmask(SIG_BLOCK, &set, NULL);
            signal(SIGFPE, SIG_DFL);
            signal(SIGPIPE, SIG_IGN);
            // signal(SIGSTOP, SIG_IGN); // cannot be caught
            signal(SIGSEGV, SIG_IGN);
            break;

        case 2:
            // catch SIGPIPE/SIGSTOP (SIGSTOP cannot be caught) and block SIGFPE and SIGSEGV
            sigaddset(&set, SIGPIPE);
            sigprocmask(SIG_BLOCK, &set, NULL);
            signal(SIGFPE, SIG_IGN);
            signal(SIGPIPE, SIG_DFL);
            // signal(SIGSTOP, SIG_DFL); // cannot be caught
            signal(SIGSEGV, SIG_IGN);
            break;

        case 3:
            // catch SIGSEGV and block SIGFPE and SIGPIPE
            sigaddset(&set, SIGSEGV);
            sigprocmask(SIG_BLOCK, &set, NULL);
            signal(SIGFPE, SIG_IGN);
            signal(SIGPIPE, SIG_IGN);
            // signal(SIGSTOP, SIG_IGN); // cannot be caught
            signal(SIGSEGV, SIG_DFL);
            break;
    }

    // Compute sum
    for (i = 0; i <= 1000 * tid; i++) {
        sum += i;
    }
    printf("Thread %d computed sum = %d\n", tid, sum);
    time_t start_time = time(NULL);

    // I was trying to override the sigwait by time
    while(difftime(time(NULL), start_time) <= 25) {
        int sig;
        sigwait(&set, &sig); // Blocking, so I dont know how to Exit out of this loop
        printf("Thread %d caught signal %d\n", tid, sig);
        if (sig == SIGUSR1) {
            // call signal handler
            break;
        }
    }

    // Alternate while loop implementation using sigtimedwait 
    // int sig;
    // struct timespec timeout;
    // timeout.tv_sec = 25; // 25 second timeout
    // timeout.tv_nsec = 0;

    // int result = sigtimedwait(&set, &sig, &timeout);
    // if (result == -1) {
    //     exit(EXIT_FAILURE);
    // } else {
    //     printf("Thread %d caught signal %d\n", tid, sig);
    //     if (sig == SIGUSR1) {
    //         // call signal handler
    //     }
    // }

    printf("Thread %d exited\n", tid);
    pthread_exit(NULL);
}

