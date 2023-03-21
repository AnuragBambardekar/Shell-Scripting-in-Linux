#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * When the child process exits, it becomes a Zombie process because its exit status is still stored in the process table, 
 * but its parent process has not yet called wait() to retrieve the exit status. During the 10 seconds that the parent process is sleeping, 
 * the child process will be in a Zombie state.
*/


int main() {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    else if (child_pid == 0) {
        printf("Child process: PID=%d\n", getpid());
        exit(0);
    }
    else {
        printf("Parent process: Created child with PID=%d\n", child_pid);
        sleep(10);
        printf("Parent process: Exiting\n");
    }

    return 0;
}