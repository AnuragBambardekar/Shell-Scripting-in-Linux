#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// Nothing displayed
// Analysed this with pen and paper
// Child #2 is blocked on read() forever
// Child #0 and #1 are blocked on wait() forever

void Fn(int i, int j) {
    // TODO: implement the xn() function
}

int main()
{

    int fd[3][2], i, j, a, ret;
    pipe(fd[0]); // pipe 0
    pipe(fd[1]); // pipe 1
    pipe(fd[2]); // pipe 2
    for (i = 0; i < 3; i++) {
        ret = fork();
        if (ret != 0) // Parent scope
            continue; // continue passes control to the next iteration of the nearest
                      //enclosing do/for/while statement in which it appears

        // Child Scope Starts
        switch (i) {
            case 0:
            a = 3; write(fd[2][1], &a, sizeof(a)); break; // child 0 - write to pipe 2, then waits on read() from pipe 0
            case 1:
            a = 2; write(fd[1][1], &a, sizeof(a)); break; // child 1 - write to pipe 1, then does read() from pipe 1
            case 2:
            a = 4; // child 2
            read(fd[2][0], &a, sizeof(a)); // child 2 - read from pipe 2
            write(fd[0][1], &a, sizeof(a)); // child 2 - write to pipe 0, then does read() from pipe 3 - forever blocked!
        }
        read(fd[i][0], &a, sizeof(a)); // Line A, chils 0 reads from pipe 0
        for (j = 0; j < a; j++) {
            ret = fork(); // children processes are forked
            if (ret == 0) {
                Fn(i, j); exit(0); // Fn is assumed to never return
            }
        }
        for (j = 0; j < a; j++) wait(NULL); // Line B // Wait on any child to terminate, No Children (grandchildren) of Child process returns, since Fn is never returning 
    }
    // Parent Scope
    for (i = 0; i < 3; i++) wait(NULL); // Line C // Wait on any child to terminate, No child terminates (specifically child #0 and #1 never do, Child #2 is blocked on read())
        a = 1; 
        write(fd[2][1], &a, sizeof(a)); // Line D

}
