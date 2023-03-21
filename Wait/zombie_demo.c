// When a child process terminates, but its parent does not wait for it,
// the process becomes known as a zombie (aka defunct)

// Child processes must report to their parents before their resources will be
// released by the OS

// Purpose of a zombie process is to retain the state that wait() can retrieve
// they want to be harvested

//Reaping the orphan zombies
// If a parent process terminates without cleaning up its zombies, the zombies
// become orphan zombies

// orphans are adopted by init process (1) which periodically waits() for orphans
// Thus, eventually the orphan zombies die

// Let's make a zombie
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main()
{
    pid_t spawnPID = -5;
    int childExitStatus = -5;

    spawnPID = fork();
    switch(spawnPID)
    {
        case -1:
        perror("Hull BREACH!\n");
        exit(1);
        break;

        case 0:
        printf("CHILD: Terminating!");
        break;

        default:
        printf("PARENT: making child a zombie for 10 seconds\n");
        printf("PARENT: Type \"ps -elf | grep \'username\'\" to see the defunct child\n");
        printf("PARENT: Sleeping...\n");
        fflush(stdout); //make sure all text is outputted before sleeping
        sleep(10);
        waitpid(spawnPID, &childExitStatus, 0);
        break;
    }
    printf("This will be executed by both of us!\n");
}