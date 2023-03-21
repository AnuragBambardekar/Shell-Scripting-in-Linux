// Forking a child process
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main()
{
    pid_t pid;
    pid = fork();

    if(pid<0){
        fprintf(stderr, "Fork Failed!");
        return 1;
    }
    else if (pid == 0) {
        // Child
        execlp("/bin/ls","ls",NULL);
    }
    else {
        // Parent
        // suspends the execution of the calling process until one of its child processes terminates
        wait(NULL); 
        // NULL argument indicates that the exit status of the terminated child 
        // process will not be returned. If the parent process needs to know the exit status of the child process, it can pass a pointer to 
        // a variable where the exit status will be stored as an argument to the wait() function
        printf("Child Complete.\n");
    }
    return 0;
}