#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid, pid1;

    pid = fork();
    printf("%d\n",pid);
    if(pid<0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid==0)
    {
        pid1 = getpid();
        //wait(NULL); //parent executes first then child, doesn't really make a difference
        //sleep(1);
        printf("child1: %d\n",pid);  //0
        printf("child2: %d\n",pid1); //31346
    }
    else
    {
        pid1 = getpid();
        wait(NULL); //waits for child
        printf("parent1: %d\n",pid);  //31346
        printf("parent2: %d\n",pid1); //31345
        //wait(NULL);   //doesn't really wait for child

    }

    return 0;
}