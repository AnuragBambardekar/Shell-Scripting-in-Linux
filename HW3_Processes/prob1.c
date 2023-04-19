#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void f1(int arg1, int arg2) {
    // function body
    printf("Called!");
    while(1) {
        // do something indefinitely
    }
}

int pg[2];
void handler(int signum) { 
    printf("Handler called!\n");
    exit(4); 
    f1(11, -1); 
}
int main(void) {
    int i, j, n; char c[10]; pid_t pid[4]; //variable declarations
    signal(SIGUSR1, handler);
    pipe(pg);
    for (i = 0; i < 5; i++) {
        pid[i] = fork();
        printf("PID: %d %d, Iteration: %d\n", getpid(), getppid(), i);
        if (pid[i] == 0) {
            sleep(3);
            printf("In child: PID: %d %d\n", getpid(), getppid());
            n = read(pg[0], c, 2*i+1);
            if (i<3) {
                printf("if condition: PID: %d %d\n", getpid(), getppid());
                exit(2-i); }
            else {
                printf("else condition: PID: %d %d\n", getpid(), getppid());
                f1(i,n);}
            }
    }
    fflush(stdout);

    kill(pid[i - 2], SIGUSR1);
    for (i = 0; i < 5; i++) {
        wait(&n);
        write(pg[1], c, WEXITSTATUS(n));
    }
    f1(i, -1);
    return 0;
}