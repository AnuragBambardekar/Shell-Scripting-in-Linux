#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> //remove this if you want to call waitpid or define it properly

int main(int arc, char *argv[])
{
    int i,j,k,status;
    pid_t pid, pid_real;
    int res;
    char cmd[200];
    k=0;
    pid=getpid();
    fprintf(stdout,"The initial process pid is=%d\n",pid);

    int p =fork();
    
    for(i=0;i<=p;i++){
        // fork();
        waitpid(p, &status, 0);
        p=fork();
        if(i==16)
            exit(0);
    } 

    // For k to increment process-wise perhaps use semaphores
    k++;
    fprintf(stdout,"I am process %d and k is %d\n", (int)getpid(),k);
    pid_real = getpid();

    sprintf(cmd,"pstree -np -C age %d", pid_real);
    
    res = system(cmd);

    if (res<0) fprintf(stdout, "The pstree command did not execute.\n");

    return 0;
}