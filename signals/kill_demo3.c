#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdarg.h>

void handler(int sig) {
    printf("Received signal %d\n", sig);
}

int main() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        printf("Child process (PID %d) started\n", getpid());

        printf("Child process (PID %d) is waiting for SIGSTOP signal\n", getpid());

        handler(SIGSTOP);

        printf("Child process (PID %d) received SIGCONT signal\n", getpid());

        printf("Child process (PID %d) is terminating\n", getpid());
        exit(0);
    }

    printf("Parent process (PID %d) started, child process has PID %d\n", getpid(), pid);

    sleep(1);

    printf("Parent process (PID %d) sending SIGSTOP signal to child process (PID %d)\n", getpid(), pid);
    kill(pid, SIGSTOP);


    printf("Parent process (PID %d) sending SIGCONT signal to child process (PID %d)\n", getpid(), pid);
    kill(pid, SIGCONT);

    waitpid(pid, &status, 0);

    printf("Parent process (PID %d) received status %d from child process (PID %d)\n", getpid(), status, pid);

    return 0;
}
