#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

// re-installing custom handler
// DFL -> CustomhHandler
// To restore default signal handler, specify SIG_DFL as signal handler
// There's a portability problem as signal() behaves differently on different OS
// sigaction() does everything signal() does and is better.

void signalhandler(int signum)
{
    int got_sigusr1 = 1;
    fprintf(stderr, "got signal number: %d", signum);
    signal(signum, signalhandler);
}

int main()
{
    if (signal(SIGUSR1, signalhandler)<0 )
    {
        perror("could not establish SIGUSR handler");
        exit(1);
    }
    sleep(10);
    return 0;
}