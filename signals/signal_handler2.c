#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
void sig_handler (int signum)
{
    fprintf(stderr,"Received signal %d\n", signum);
    // exit(signum);
}
 
int main()
{ 
    struct timespec remaining, request = {1, 34};
    signal(SIGINT, sig_handler);
    signal(SIGSTOP, sig_handler);
    // signal(SIGTSTP, sig_handler);
    signal(SIGSEGV, sig_handler);
    signal(SIGQUIT, sig_handler);
    signal(SIGABRT, sig_handler);
    // signal(SIGCONT, sig_handler);
    int signum;
    int pause_time;
    pid_t pid, cpid;
    int i;
 
    pid=getpid();
    fprintf(stderr,"I am program R_Pr1 and my pid is: %d\n",pid);
    int status;
    pause_time=20;
    signum=-1;
 
    cpid=fork();
    for (i=0; i<20; i++) {
        // while(1) {
 
        fprintf(stderr,"Another loop iteration before sleep for 20+ = %d seconds for process=%d \n",pause_time, getpid());
 
        sleep(pause_time); // This is your chance to press CTL-C
        //pause();
 
        //The fprintf below will only give you 0 as the value of signal. 
        //There are certain macros that are required to process the signal
        // collect the signal number and print it. 
        // One way is through the handler function signal
        //There are also macros like sigqueue and ismember functions.
        // But we cannot do so simply through a printout from main.
        //So the fprintf below will always give you 0.
        pause_time+=2;
        fprintf(stderr,"Processing stuff here but also received signal = %d\n",signum);
    }
    wait(&status);
    if (WIFEXITED(status)) 
    {
        fprintf(stderr,"Process %d Exited naturally, status = %d\n",cpid,WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {fprintf(stderr,"Process %d Received signal: %d\n",cpid,WTERMSIG(status));}
    else if (WIFSTOPPED(status)) {fprintf(stderr,"Process %d Stopped by signal %d\n",cpid,WSTOPSIG(status));}
    else if (WIFCONTINUED(status)) {fprintf(stderr,"Process %d Continued by signal %d\n",cpid,WTERMSIG(status));}
 
 return 0;
}