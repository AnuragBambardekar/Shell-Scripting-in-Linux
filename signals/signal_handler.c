#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

// A Custom Signal Handler


void sig_handler (int signum)
    {
        fprintf(stderr,"Received signal %d\n", signum);
 //       exit(signum); // handler will only runs once
        // if this is uncommented and SIGINT is sent 10 times, it will execute only once //IMPORTANT!!!!!!!!!!!
  }


int main()
{   
    signal(SIGINT, sig_handler);  // Catchable
    signal(SIGSTOP, sig_handler); // Not Catchable
    // signal(SIGTSTP, sig_handler); // Catchable
    signal(SIGSEGV, sig_handler); // Catchable
    signal(SIGQUIT, sig_handler); // Catchable
    signal(SIGABRT, sig_handler); // Catchable
    // signal(SIGCONT, sig_handler); // Catchable

    int signum;
    int pause_time;
    pid_t pid, cpid;
    int i;

    pid=getpid();
    fprintf(stderr,"I am program R_Pr1 and my pid is: %d\n",pid);

    pause_time=20;

    for (i=0; i<20; i++)  {
        // while(1) {
        fprintf(stderr,"Another loop iteration before sleep for 20+ = %d seconds\n",pause_time);

        sleep(pause_time);    // This is your chance to press CTL-C

        //The fprintf below will only give you 0 as the value of signal. 
        //There are certain macros that are required to  process the signal
        // collect the signal number and print it. 
        // One way is through the handler  function signal
        //There are also macros like sigqueue and ismember functions.
        // But we cannot do so simply through a printout from main.
        //So the fprintf below will always give you 0.
        pause_time+=2;
        fprintf(stderr,"Processing stuff here but also received signal = %d\n",signum);
    } 
   return 0;
}
