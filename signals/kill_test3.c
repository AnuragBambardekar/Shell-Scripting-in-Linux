#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

void sigtstp_handler(int signum) {
    printf("Child process stopped by signal: %d\n", signum);
}

void sigcont_handler(int signum) {
    printf("Child process resumed by signal: %d\n", signum);
}

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Failed to fork process");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process code
        printf("Child process running\n");
        // sleep(60);
        // exit(42);
        signal(SIGTSTP, sigtstp_handler);
        signal(SIGCONT, sigcont_handler);
        while(1);
    } else {
        // Parent process code
        printf("Parent process waiting for child\n");
        int status;
        pid_t result = waitpid(pid, &status, 0);

        if (result == -1) {
            perror("Failed to wait for child process");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("Exited with status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Process received a signal number: %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("Process is stopped! by signal: %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("Process is resumed!\n");
        }
    }

    return 0;
}
