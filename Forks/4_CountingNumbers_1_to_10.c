#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int id = fork(); //child process created

    //both parent and child execute the instructions from here onwards
    int n;

    // check if in main process
    if(id==0){
        n=1;
    } else {
        n=6;
    }

    //Count from 1 - 10
    //underlying OS decides the order
    //now it prints:
    //678910
    //12345

    //so, we then use wait() --> stop the execution until child process has finished executing
    if(id!=0)
    {
        wait(NULL);
    }

    for(int i=n; i<n+5; i++)
    {
        printf("%d ",i);
        // fflush(stdout); //output properly shows up on terminal
    }
    if(id!=0)
    {
        printf("\n");
    }

	return 0;
}
