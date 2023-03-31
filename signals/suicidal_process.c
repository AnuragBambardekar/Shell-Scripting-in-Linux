#define _POSIX_C_SOURCE 1

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>


int main (void)
{
    pid_t pid = getpid(); /* Get my iown process ID. */

    // kill(pid, SIGKILL); /* Send myself a KILL signal. */

    puts("Signal delivery initiated.");  /* Although not guaranteed, practically the program never gets here. */
    // fflush(stdout);

    kill(pid, SIGKILL); /* Send myself a KILL signal. */

    pause(); /* Wait to die. */

    puts("This never gets printed.");
}