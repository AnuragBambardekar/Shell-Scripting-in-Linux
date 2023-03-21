#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// BEWARE: FORK BOMB

int main(int argc, char *argv[])
{
    int p = fork(); //returns 0
    int status;
    // printf("%d\n",p);
    for(int i=0; i<=p; i++)
    {
        // waitpid(p,&status, 0); //proper implementation, however i earlier used waitpid(p); just remember to not include sys/wait header file
        printf("GrPr: %d, Pr: %d, num: %d, iter: %d\n",getppid(), getpid(), p, i);
        p=fork();
        printf("hello: %d\n",p);
        // if (i==1)
        //     exit(0);
    }
    return 0;
}

// 1. Anticipated output?
/**
 * The program will run in an infinite loop that generates a new child process
 * in each iteration i.e., as long as fork() system call is successful (it returns value > 0).
 * The program will run indefinitely until it is manually terminated by the user.
 * The waitpid(p) is a system call and it causes the parent process to wait
 * for the child process to complete before continuing with the loop
 * and executing the next instructions.
 * 
 * In the first iteration:
 * p = fork() --> shell process forks a child, so p=0 and p>0 for parent process
 * So, loop runs only 1 time for the child process and printf() statement prints:
 * GrPr: 21012, Pr: 21013, num: 0, iter: 0
 * GrPr: 21012, Pr: 21013, num: 21014, iter: 1
 * GrPr: 21012, Pr: 21013, num: 21015, iter: 2
 * GrPr: 21012, Pr: 21013, num: 21016, iter: 3
 * 
 * The loop will run indefinitely for the parent process, forking a new child process
 * in each iteration.
 * so p value in each iteration will be the return value of fork() in each iteration
 * However, GrPr and Pr values remain the same for each process. (it is apparently the case, but in reality, child processes are also concurrently executing,
 * it is just that the terminal doesn't catch up with infinite processes.)
 * 
 * 
 *  GrPr: 4456, Pr: 4457, num: 0, iter: 0
 *  hello: 4458
 *  hello: 0
 *  GrPr: 4456, Pr: 4457, num: 4458, iter: 1
 *  hello: 4459
 *  GrPr: 3227, Pr: 4456, num: 4457, iter: 0
 *  hello: 4460
 *  hello: 0
 *  hello: 0
 *  GrPr: 3227, Pr: 4456, num: 4460, iter: 1
 *  hello: 4461

*/

// 2. Does the program end or does it run infinitely? 
// The program runs infinitely.

// 3. Is this a fork bomb (the number of fork processes keeps growing during execution and as a result system resources are depleted)?
// Yes, this is a fork bomb, which is a type of denial-of-service attack that uses a 
// process to repeatedly replicate itself and consume system resources
// it can slow down or crash the system due to resource starvation
// It is also called rabbit virus or wabbit.

// 4. How many processes are forked? How many processes run concurrently at any time?
/**
 * The number of processes that are forked corresponds to the value of 'p' and
 * also the number of iterations 'i'.
 * So, since the loop runs from i=0 to i<=p, so the number of processes
 * that will be forked is p+1 child processes.
 * 
 * The number of processes that can run concurrently at any time is 2 [the parent  process
 * and the child process], because in the given program the parent process
 * waits for the child process to finish execution. The parent does this by
 * invoking the waitpid() system call.
 * 
 * So, at any given time only one child process will be running concurrently
 * with the parent process.
 * 
 * Additionally, the number of concurrently executing processes also depends
 * on the CPU configuration (number of cores, process scheduling algorithm).
*/

