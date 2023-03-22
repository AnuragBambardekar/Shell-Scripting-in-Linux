// #define _POSIX_SOURCE
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void create_child_process(int sleep_val) {
    pid_t cpid;

    cpid = fork();
    if (cpid == -1) {
        perror("Fork Error");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        printf("Child Process PID is: %d\n", getpid());
        sleep(sleep_val);

        printf("Child with PID: %d Exited\n", getpid());
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[]) {

    int status = 0;
    create_child_process(60);

    // continuously monitors the status of each child process using the waitpid() system call
    while(1){
        int w = waitpid(-1, &status, WNOHANG| WUNTRACED); // wait for every child process, non-blocking call

        if(w==-1) {
            break;
        }

        if(w>0) {
            //monitor the exit state and capture it
            if(WIFEXITED(status)) {
                printf("Exited with status: %d\n", WEXITSTATUS(status));
            } else if(WIFSIGNALED(status)) {
                printf("Process received a signal number: %d\n", WTERMSIG(status));
            } else if(WIFSTOPPED(status)) {
                printf("Process is stopped! by signal: %d\n", WSTOPSIG(status));
            } else if(WIFCONTINUED(status)) {
                printf("Process is resumed!\n");
            } 
        }
    }

    printf("No Child process. Exit Parent\n"); //if no processes to monitor
    return 0;
}