#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    //always create processes from parent process
    int id1 = fork();
    int id2 = fork();

    if(id1 == 0){
        if(id2 == 0){
            printf("Process Y\n");
        } else {
            printf("Process X\n");
        }
    } else {
        if(id2 == 0){
            printf("Process Z\n");
        } else {
            printf("Parent Process\n");
        }
    }


    while(wait(NULL) != -1 || errno != ECHILD){
        printf("Waited for a child to Finish\n"); //should be printed 3 times
    }

    return 0;
}