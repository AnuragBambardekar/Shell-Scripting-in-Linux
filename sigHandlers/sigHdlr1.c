#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


void chldhandler() {
    int pid1, status;
    pid1 = wait(&status);
    printf("Child done in time\n");
    sleep(5); // debugging why i'm getting 2 child done in time printed.
    exit(status);
}
void main(int argc, char* argv[]) {
    int pid;
    signal(SIGCHLD, chldhandler);
    pid = fork();
    if (!pid) {
        printf("I am child and my PID: %d, and my parent: %d\n", getpid(), getppid());
        fflush(stdout); // had to add fflush because execvp was getting executed first
        sleep(2);
        // if (execvp(argv[2], &argv[2]) == -1) { // when successful, child terminates and SIGCHLD is sent to parent and 'Child done in time' is printed!
        if (execvp("/path/to/non-existent-file", &argv[2]) == -1) {
            
            perror("execvp failed");
            printf("Waiting for someone to send some signals\n");
            sleep(10); // so that I can send SIGCHLD from another terminal
            exit(EXIT_FAILURE);
        }
    }
    else {
        // parent
        sleep(10);
        printf("Child too slow\n");
        kill(pid, SIGINT);
    }
}