#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
# include <bits/sigaction.h> // for sigaction handler
#ifndef __sigset_t_defined
#define __sigset_t_defined 1

#include <bits/types/__sigset_t.h>

/* A set of signals to be blocked, unblocked, or waited for.  */
typedef __sigset_t sigset_t;
#endif

#define THREAD_COUNT 3
#define NUM 1000

pthread_t tid[THREAD_COUNT];

int should_exit = 0;

void* signal_handler(void* arg) {
    int thread_num = *(int*)arg;
    int sum = 0;
    sigset_t set;
    sigemptyset(&set);

    switch(thread_num) {
        case 1:
            // catch SIGFPE and block SIGSTOP and SIGSEGV
            sigaddset(&set, SIGFPE);
            sigprocmask(SIG_BLOCK, &set, NULL);
            signal(SIGFPE, SIG_DFL);
            signal(SIGSTOP, SIG_IGN);
            signal(SIGSEGV, SIG_IGN);
            break;

        case 2:
            // catch SIGSTOP and block SIGFPE and SIGSEGV
            sigaddset(&set, SIGSTOP);
            sigprocmask(SIG_BLOCK, &set, NULL);
            signal(SIGFPE, SIG_IGN);
            signal(SIGSTOP, SIG_DFL);
            signal(SIGSEGV, SIG_IGN);
            break;

        case 3:
            // catch SIGSEGV and block SIGFPE and SIGSTOP
            sigaddset(&set, SIGSEGV);
            sigprocmask(SIG_BLOCK, &set, NULL);
            signal(SIGFPE, SIG_IGN);
            signal(SIGSTOP, SIG_IGN);
            signal(SIGSEGV, SIG_DFL);
            break;
    }

    // printf("Thread #: %d\n", thread_num);
    for (int i = 0; i <= NUM * thread_num; i++) {
        sum += i;
    }
    printf("Thread %d computed sum: %d\n", thread_num, sum);

    // while(1) { // infinite loop
    printf("Threads will stay alive for 30 seconds, so send signals to be handled by the 3 threads.\n");
    
    while(!should_exit) {
        int sig;
        sigwait(&set, &sig);
        printf("Thread %d caught signal %d\n", thread_num, sig);

        should_exit = 1; // make each thread exit after handling the signal once
    }

    sleep(30);
    pthread_exit(NULL);
    printf("Thread %d Exited\n", thread_num);
    return NULL;
}

void main_signal_handler(int sig) {
    printf("Main caught signal %d\n", sig);
}

int main() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGFPE);
    sigaddset(&set, SIGSTOP);
    sigaddset(&set, SIGSEGV);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // signal(SIGFPE, main_signal_handler);
    // signal(SIGSTOP, main_signal_handler);
    // signal(SIGSEGV, main_signal_handler);

    // signal(SIGFPE, SIG_IGN);
    // signal(SIGSTOP, SIG_IGN);
    // signal(SIGSEGV, SIG_IGN);

    int thread_args[THREAD_COUNT];

    printf("Waiting for signals to be received! (Going to be ignored)\n");
    fflush(stdout);
    // wait to send signals while program is registered with the initial signal disposition
    sleep(25);

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_args[i] = i + 1;
        pthread_create(&tid[i], NULL, signal_handler, &thread_args[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Waiting to receive signals to be handled by SIG_DFL\n");
    sleep(25);
    signal(SIGFPE, SIG_DFL);
    signal(SIGSTOP, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);

    fflush(stdout);
    printf("Program Exited!\n");
    
    exit(0);
    return 0;
}
