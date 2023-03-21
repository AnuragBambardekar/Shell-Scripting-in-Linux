#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>

// Testing different options of waitpid
// Uncomment each block to see different outputs

int main(int argc, char *argv[])
{
    int i, status;
    pid_t pid;
    int res, resWait;
    const char *cmd;

    pid = fork();
    printf("After Fork: %d\n",pid);

    if(pid<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if (pid>0)
    {
        //Parent process
        printf("This is Parent Process and my PID is: %d and my Pr: %d, GrPr: %d\n",pid, getpid(),getppid());
        //cmd = "/bin/ls";
        //cmd = "./prog1";
        cmd ="/home/anuragb/ECE434/HW2_Forks/prog3";
        char *arguments[] = { NULL };

        // waitpid(pid, &status, 0) --> Make parent always wait for child process to finish
        // resWait = waitpid(pid, &status, 0);
        // if(resWait == -1)
        // {
        //     perror("waitpid failed");
        //     exit(1);
        // }
        // if(WIFEXITED(status))
        // {
        //     printf("Child process exited with status %d\n", WEXITSTATUS(status));
        // }

        // printf("Proof that child executed above me and, then exec()\n\n");

        // WNOHANG
        // resWait = waitpid(pid, &status, WNOHANG);
        // if (resWait == -1) {
        //     fprintf(stderr, "waitpid() returned error\n");
        // }
        // else if (resWait == 0) {
        //     fprintf(stderr, "Parent is not waiting for child to exit\n\n");
        // }
        // printf("Parent keeps on Executing regardless of child's termination status.\n\n");


        // waitpid(0, &status, 0) --> If PID is 0, match any process with the same process group as the current process.
        // so parent process waits for any child process to finish execution/ terminate
        resWait = waitpid(0, &status, 0);
        if(resWait == -1)
        {
            perror("waitpid failed");
            exit(1);
        }
        if(WIFEXITED(status))
        {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        }

        printf("A break line for neat output.\n\n");


        res = execvp(cmd, arguments); //replace parent's memory content with prog3
        if (res==-1) fprintf(stderr,"Parent did not execute execl\n");
    }
    else
    {
        //Child process
        printf("yo: %d\n",pid);
        fprintf(stderr,"Child says Hello, I am Child and my PID is: %d and parent's PID is: %d\n",getpid(), getppid());
        // resWait = waitpid(pid, &status, 0);
        // if (resWait == -1) {
        //     fprintf(stderr, "waitpid() returned error\n\n");
        // }
        // else if (resWait == 0) {
        //     fprintf(stderr, "Child is waiting for a non-child process to exit\n");
        // }
        // else {
        //     fprintf(stderr, "Child waited for non-child process with pid %d\n", resWait);
        // }

        // resWait = waitpid(0, &status, 0);
        // if(resWait == -1)
        // {
        //     perror("waitpid failed");
        //     exit(1);
        // }
        // if(WIFEXITED(status))
        // {
        //     printf("Child process exited with status %d\n", WEXITSTATUS(status));
        // }

        // printf("Proof that what happened?\n\n");
    }
    return 0;
}