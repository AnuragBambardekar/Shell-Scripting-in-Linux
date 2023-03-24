#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

//Execution
// anuragb@anuragb:~/ECE434/pipes$ g++ 5c_PipeLoop_inclassprob3.cpp -o 5c_PipeLoop_inclassprob3
// anuragb@anuragb:~/ECE434/pipes$ ./5c_PipeLoop_inclassprob3

// pipes with children and grandchildren

void Fn(int i, int j) {
    // TODO: implement the xn() function
}

int main() { 
    int i,j,n, fd[3][2]; 
    pid_t p[3]; 

    for(i=0; i<3; i++) pipe(fd[i]); //create the pipes

    for(i=0; i<3; i++)
    {
        p[i] = fork();
        if(p[i] == 0)
        {
            //child process
            for(j=i; j<3; j++)
            close(fd[j][1]);
            sleep(5); // not in for loop
            n=3-i; // not in for loop
            if (i>0)
            write(fd[i-1][1], &n, sizeof(n));
            read(fd[i][0], &n, sizeof(n));

            for(j=0;j<n;j++)
                if(fork() == 0)
                    Fn(i,j);
            wait(NULL);
            Fn(i,5);
        }
    }

    n=2;
    write(fd[2][1], &n, sizeof(n));
    kill(p[2], SIGKILL);
    close(fd[1][1]);
    wait(NULL);
    Fn(i,n);
    
    return 0;
}
