#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void signal_handler(int signal) {
    printf("Received signal: %d\n", signal);
}

int main() {
    pid_t pid;
    int status;

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        printf("Error: failed to fork process\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process starting...\n");
        printf("Child process waiting for SIGCONT signal...\n");

        // Set up signal handler for SIGCONT
        signal(SIGCONT, signal_handler);

        // Wait for the signal
        pause();

        printf("Child process resumed!\n");
        return 0;
    } else {
        // Parent process
        printf("Parent process starting...\n");

        // Wait for a short time to ensure child process is paused
        sleep(1);

        printf("Sending SIGCONT signal to child process...\n");

        // Send SIGCONT signal to child process
        kill(pid, SIGCONT);

        // Wait for child process to exit
        wait(&status);

        printf("Child process exited with status: %d\n", status);
        printf("Parent process exiting...\n");
        return 0;
    }
}
