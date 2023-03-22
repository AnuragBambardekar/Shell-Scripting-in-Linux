#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Web Server listener

int main(int argc, char* argv[]) {
    int pid = fork();

    if(pid == -1) {
        return 1;
    }

    if(pid==0) {
        // Child Process

        //Create an infinite loop
        while(1) {
            printf("Hello\n");
            sleep(1);
        }
    } else {
        // sleep(1); // Prints hello once
        // kill(pid, SIGKILL); //then kill it

        // Stop the process - Nothing prints 
        // (child tries to execute but parent stops it abruptly)
        // kill(pid, SIGSTOP);
        // sleep(1);
        // kill(pid, SIGKILL);


        // kill(pid, SIGSTOP);
        // sleep(1);
        // kill(pid, SIGCONT);
        // sleep(1);
        // kill(pid, SIGKILL);

        // Real life Implementation : Web Server listener
        kill(pid, SIGSTOP);
        int time;
        do {
            printf("Time in seconds for execution: ");
            scanf("%d", &time);

            if(time>0) {
                kill(pid, SIGCONT);
                sleep(time); //sleep for 'time' seconds
                kill(pid, SIGSTOP);
            }
        } while(time>0);

        kill(pid, SIGKILL);

        wait(NULL);
    }

    return 0;
}