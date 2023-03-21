#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// fork() + execlp() example
void main()
{
    pid_t spawnPID = 5;
    int childExitStatus = -5;

    spawnPID = fork();
    switch(spawnPID)
    {
        case -1:
        perror("Hull Breach!");
        exit(1);
        break;

        case 0:
        printf("CHILD(%d): Sleeping for 1 second\n", getpid());
        sleep(1);
        printf("CHILD(%d): Converting into \'ls -a\'\n", getpid());
        execlp("ls", "ls", "-a", NULL);
        perror("CHILD: exec failure!\n");
        exit(2);
        break;

        default:
        printf("PARENT(%d): Sleeping for 2 second\n", getpid());
        sleep(2);
        printf("PARENT(%d): Wait()ing for child(%d) to terminate\n", getpid(), spawnPID);
        pid_t actualPID = waitpid(spawnPID, &childExitStatus, 0);
        printf("PARENT(%d): Child(%d) terminated, Exiting!\n", getpid(), actualPID);
        exit(0);
        break;
    }
}