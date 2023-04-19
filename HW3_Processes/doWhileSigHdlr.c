#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
# include <bits/sigaction.h> // for sigaction handler
#ifndef __sigset_t_defined
#define __sigset_t_defined 1

#include <bits/types/__sigset_t.h>

/* A set of signals to be blocked, unblocked, or waited for.  */
typedef __sigset_t sigset_t;

#endif
#include <sys/types.h>
#include <sys/wait.h>

volatile int num_children = 5;

void sigchld_handler(int signo)
{
    pid_t pid;
    int status;

    do {
        pid = waitpid(-1, &status, WNOHANG);
        // sleep(5);
        if (pid > 0) {
            printf("Child process %d terminated with status %d\n", pid, WEXITSTATUS(status));
            num_children--;
        }
        // if(WEXITSTATUS(status) == 4) {exit(0);}
    } while (pid > 0); // || pid == pid+4
    // exit(0);

    if (num_children == 0) {
        printf("All child processes have terminated\n");
        exit(0);
    }
}

int main()
{
    // Install the SIGCHLD signal handler
    //earlier i didnt have these 3 lines
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sa.sa_flags = SA_NOCLDSTOP;
    // In earlier versions of Linux, some system calls would be automatically restarted by the kernel if they were interrupted by a signal. However, this behavior could lead to subtle bugs and race conditions in some programs, so the SA_RESTART flag was introduced to disable the automatic restart behavior for certain signal handlers.

    // In modern Linux systems, most system calls are now automatically restarted by default, unless they are explicitly marked with the SA_RESTART flag to disable the behavior. Therefore, in most cases, the SA_RESTART flag is no longer necessary or recommended.

    // In the case of the SIGCHLD signal handler in the example program, the SA_RESTART flag is not needed because the waitpid function is automatically restarted by the kernel if it is interrupted by a signal. Therefore, you can safely remove the SA_RESTART flag from the sa_flags field of the sigaction structure in the program.
    

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Fork some child processes
    int i;
    for (i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            printf("Child process %d starting\n", getpid());
            sleep(i+10);
            printf("Child process %d exiting with status %d\n", getpid(), i);
            exit(i);
        }
    }

    // Wait for all child processes to terminate
    // while (1) {
    //     pause(); // waiting for a signal
    //     printf("Received SIGCHLD signal\n");
    // }
    
    int num_children = 5;
    int num_reaped = 0;

    while (1) {
        pause(); // waiting for a signal
        printf("Received SIGCHLD signal\n");
        int signo;
        sigwait(&mask, &signo);
        
    }


    return 0;
}
