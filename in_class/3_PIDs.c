#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int i;
    for(i=0;i<3;i++)
    {
        fork();
        printf("[%d] [%d] i=%d\n", getppid(), getpid(), i); //8
    }
    printf("[%d] [%d] hello\n", getppid(), getpid());
    return 0;
}

/**
 * 1. loop starts in parent, i=0
 * 2. parent fork() creates Child 1
 * 3. Now we have two processes, both print i=0
 * 
 * 4. loop restarts in both processes, i=1
 * 5. parent and Child 1 fork() creates Child 2 and Child 3
 * 6. Now we have four processes, all print i=1
 * 
 * 7. loop restarts in all 4 processes, i=2
 * 8. parent and Child 1,2,3 fork() creates Child 4,5,6,7
 * 9. Now we have 8 processes, all print i=2
 * 
 * 10. loop restarts in all 8 processes, i=3
 * 11. loop terminates in all 8 processes as i<3 is no longer true
 * 12. all 8 processes print hi
 * 13. all 8 processes terminate
*/