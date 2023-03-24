#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

// A Broken Pipe Demo

int main() {
    char buffer[30];
    int pipefd[2];

    if(pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();

        if (pid > 0) { // parent
            printf("In parent with pid: %d\n", getpid());
            close(pipefd[0]); // close the read end of the pipe in the parent process
            char write_msg[25] = "Greetings";
            write(pipefd[1], write_msg, strlen((write_msg)+1));
        }
        else if (pid == 0) { // child
            printf("In child with pid: %d\n", getpid());
            close(pipefd[1]); // close the write end of the pipe in the child process
            int n = read(pipefd[0], buffer, 30); // read from the pipe in a loop until all data has been received
            buffer[n] = '\0';
            printf("Child received message: %s\n", buffer);
            exit(0);
        } else {
            printf("Fork Error");
            exit(1);
        }
    }

    for (int i = 0; i < 5; i++) {
        printf("Doing some work!");
        sleep(10);
    }

    return 0;
}
