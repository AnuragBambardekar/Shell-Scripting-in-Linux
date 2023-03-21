#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// Checking the Exit Status
void main()
{
    int childExitMethod;
    pid_t childPID = wait(&childExitMethod);

    if(childPID == -1)
    {
        perror("Wait Failed!\n");
        exit(1);
    }
    if(WIFEXITED(childExitMethod))
    {
        printf("The process exited normally\n");
        int exitStatus = WEXITSTATUS(childExitMethod);
        printf("Exit status was %d\n", exitStatus);
    }
    else
        printf("Child terminated by a signal\n");
}