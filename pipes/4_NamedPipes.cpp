#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

//To Execute:
//anuragb@anuragb:~/ECE434/pipes$ g++ 4_NamedPipes.cpp -o 4_NamedPipes
//anuragb@anuragb:~/ECE434/pipes$ ./4_NamedPipes 

int main()
{
    char buffer[30];
    int pipefd[2];

    if(pipe(pipefd) == -1)
    {
        cout<<"Error creating pipe" <<endl;
        exit(1);
    }

    for(int i=0; i<5; i++)
    {
        pid_t pid = fork();
        if(pid>0)
        {
            //parent
            cout<<"In Parent with PID: "<<getpid()<<endl;
            int dummy;
            wait(&dummy);
        } 
        else if(pid==0)
        {
            //child
            cout<<"In Child with PID: "<<getpid()<<endl;
            pid_t pid2 = fork();
            sleep(15);
            if(pid2 >0)
            {
                //child
                cout<<"Still in child process"<<endl;
            }
            else if(pid2 == 0)
            {
                //grandchild
                int n = read(pipefd[0], buffer, 12);
                buffer[n] = '\0';
                cout<<"i = "<<i<<", You typed: "<<buffer<<endl;
            }
            else{
                //fork failed
                cout<<"Error in Sub Fork"<<endl;
                exit(1);
            }
            exit(0);
        }
        else{
            cout<<"Fork Error";
            exit(1);
        }
    }

    for(int i=0; i<5; i++)
    {
        cout<<"I could be doing some work here."<<endl;
        sleep(10);
        write(pipefd[1], "hello world\n", 12);
    }
    return 0;
}

/**
anuragb@anuragb:~/ECE434/pipes$ g++ 4_NamedPipes.cpp -o 4_NamedPipes
anuragb@anuragb:~/ECE434/pipes$ ./4_NamedPipes 
In Parent with PID: 5815
In Child with PID: 5816
Still in child process
In Parent with PID: 5815
In Child with PID: 5839
Still in child process
In Parent with PID: 5815
In Child with PID: 5883
Still in child process
In Parent with PID: 5815
In Child with PID: 5930
Still in child process
In Parent with PID: 5815
In Child with PID: 5974
Still in child process
I could be doing some work here.
I could be doing some work here.
i = 0, You typed: hello world

I could be doing some work here.
i = 1, You typed: hello world

I could be doing some work here.
i = 2, You typed: hello world

I could be doing some work here.
i = 3, You typed: hello world

anuragb@anuragb:~/ECE434/pipes$ i = 4, You typed: hello world
*/