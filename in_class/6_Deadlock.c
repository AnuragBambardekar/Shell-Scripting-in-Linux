#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{   int status;
    while(1)
    {
        char *cmd = "/bin/ls";

        int p = fork();
        if(p!=0)
        {
            char *arguments[] = { NULL };
            execvp(cmd, arguments);
            printf("pid: %d",p);
        } else {
            waitpid(p,&status, 0); //Deadlock - Don't run
            // waits on a process with p=0 that doesnt exist
        }
    }
}
