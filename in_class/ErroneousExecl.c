#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <errno.h>
int main(int argc, char *argv[])  {
        int i, status,errnum;
        pid_t pid;
        pid = fork();
        if (pid<0)   {  // error occured
            fprintf(stderr,"Fork Failed");
            return 1;
        }
        else if (pid ==0)   {  //child process
            // some experiment: use home dir or current dir as 3d arg
            // execl("/usr/bin/ls","ls","-luaqweswwqewq!?","../../",NULL);
            execl("/path/to/non-existent/executable", "arg1", "arg2", NULL);
            // execl("/usr/bin/ls","ls","-lu","../../",NULL);
            perror("Erroneous execution of execl");
            fprintf(stderr,"Child did not execute execl\n");
            
         }
        else {    //parent process
            wait(NULL);
            fprintf(stderr,"Child Complete\n");
        }
   return 0;
}