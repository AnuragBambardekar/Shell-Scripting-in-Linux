#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// Practice problem on pipes
int fd[2]; // 0-READ END 1-WRITE END

//custom signal handler
void sig_hdlr1(int signum)
{ 
    printf("signal handler executed!\n");
    exit(10); 
    //xn(0, -1); 
}

int main(void)
{
    int i, j, n;
    char buf[10];
    pid_t pid[4];
    signal(SIGUSR2, sig_hdlr1);
    pipe(fd);

    for (i = 0; i < 4; i++) {
        pid[i] = fork(); //Fork a process
        printf("Inside Parent: 1st fork-> pid: %d, iter: %d\n", getpid(),i); //executed by both, since both PC's point here

        if (pid[i] == 0) {
            // child process
            printf("Inside Child: 1st fork-> pid: %d\n", getpid());
            if (i == 1) sleep(5); // put the Child #1 to sleep

            n = read(fd[0], buf, 5); //blocking, Child #0,#2,#3,#1
            // Child #0 killed by parent
            // Child #2,#3 read 5 bytes each after parent writes on WRITE_END of pipe
            // Child #1 forever blocked!

            printf("Am I unblocked now?\n");
            for (j = 0; j < i; j++) {
                pid[i] = fork();
                printf("Inside GrandChild: 2nd fork-> pid: %d, iter: %d\n", getpid(), i);
                if (pid[i] == 0)
                    //child
                    //xn(getppid(), j); // We don't know if function xn will return
                    printf("2nd fork's child: %d\n",getpid()); // So, Grandchildren of Child #2 and #3 are blocked forever!
            }
            for (j = 0; j < i; j++)
                wait(NULL);
            exit(2);
        } // child bracket closes
    } // for loop closes

    // Parent completes first loop
    
    printf("Did i Reach kill?\n");
    printf("pid[0]: %d\n",pid[0]);
    kill(pid[0], SIGUSR2); //kill Child #0 Process
    for (i = 0; i < 4; i++) 
    {
        printf("Am I here?, i:%d\n", i);
        wait(&n); // Parent waits for any child to terminate
        printf("Am I here to write?\n");
        write(fd[1], buf, WEXITSTATUS(n)); // Write exit status value on WRITE_END of pipe
        printf("I wrote to pipe!\n");
    }
    // xn(i, -1); // we don't know whether xn will return
    // fflush(stdout);
    return 0;
}
