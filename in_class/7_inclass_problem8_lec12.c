#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// runs infintely-ish (depends on the test condition)
int main(int argc, char *argv[])
{
    int status;
    int p = fork(); // forks a process

    for(int i=0; i<=p%19; i++)
    {
        waitpid(p,&status, 0);
        //printf("PID0: %d, PID1: %d, p_val: %d, i: %d\n", getppid(), getpid(), p , i);
        if(p!=0) {
            printf("PARENT ");
            printf("PID0: %d, PID1: %d, p_val: %d, i: %d\n", getppid(), getpid(), p , i);}
        else if(p==0) {
            printf("CHILD ");
            printf("PID0: %d, PID1: %d, p_val: %d, i: %d\n", getppid(), getpid(), p , i);}
        p = fork(); // executed by non-blocking processes only
    }
}