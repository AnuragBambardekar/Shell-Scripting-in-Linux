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

        while(1) {
            printf("Enter 'c' to continue child process, or 'q' to quit parent process: ");
            char input;
            scanf("%c", &input);

            if (input == 'c') {
                printf("Sending SIGCONT signal to child process...\n");
                // Send SIGCONT signal to child process
                kill(pid, SIGCONT);
            } else if (input == 'q') {
                printf("Quitting parent process...\n");
                // Send SIGKILL signal to child process
                kill(pid, SIGKILL);
                // Wait for child process to exit
                wait(&status);
                printf("Child process exited with status: %d\n", status);
                break;
            } else {
                printf("Invalid input. Try again.\n");
            }

            // Flush input buffer
            while ((input = getchar()) != '\n' && input != EOF);
        }

        printf("Parent process exiting...\n");
        return 0;
    }
}
