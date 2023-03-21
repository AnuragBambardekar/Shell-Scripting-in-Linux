#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// Proper Waitpid placement
void main()
{
    pid_t spawnPID = -5;
    int childExitMethod = -5;

    spawnPID = fork();
    if(spawnPID == -1)
    {
        perror("Hull Breach!\n");
        exit(1);
    }
    else if(spawnPID == 0)
    {
        printf("CHILD: PID: %d, exiting!\n", spawnPID);
        exit(0);
    }
    printf("PARENT: PID: %d, waiting...\n", spawnPID);
    waitpid(spawnPID, &childExitMethod, 0);
    printf("PARENT: Child Process terminated, exiting!\n");
    exit(0);
}