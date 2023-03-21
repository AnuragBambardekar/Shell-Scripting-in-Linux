#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid=fork();
    if(pid<0){
        fprintf(stderr, "FORK FAILED!");
        return 1;
    }
    else if (pid==0){
        // execlp("/bin/pstree","pstree",NULL);
        execlp("/bin/ls","ls",NULL);
    }
    else {
        wait(NULL);
        printf("Child Process Completed!");
    }
    return 0;
}