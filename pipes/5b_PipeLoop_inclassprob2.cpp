#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

//Execution
// anuragb@anuragb:~/ECE434/pipes$ g++ 5b_PipeLoop_inclassprob2.cpp -o 5b_PipeLoop_inclassprob2
// anuragb@anuragb:~/ECE434/pipes$ ./5b_PipeLoop_inclassprob2 

// pipes with children and grandchildren
int main() { 
    char buffer[30]; 
    int pipefd[2]; 
    if (pipe(pipefd) == -1) { 
        cout << "error creating pipe" << endl; 
        exit(1); 
    } 
    for (int i = 0; i < 5; i++) { 
        pid_t pid = fork(); 
        if (pid > 0) { // parent 
            cout << "in parent with pid " << getpid() << endl; 
            int dummy;
            wait(&dummy);
        } 
        else if (pid == 0) { // child 
            cout << "in child with pid " << getpid() << endl; 
            pid_t pid2 = fork();
            sleep(15);

            if(pid2 > 0)
            {
                cout<< "Still in Child process."<<endl;
            }
            else if (pid2==0)
            {
                int n = read(pipefd[0], buffer, 12); 
                buffer[n] = '\0'; 
                cout << "i = "<<i<<", You typed '" << buffer << "'" << endl; 
            }
            else
            {
                cout<<"Error in Sub Fork."<<endl;
                exit(1);
            }
            exit(0); 
        } else { 
            cout << "fork error"; 
            exit(1); 
        } 
    } 

    for (int i = 0; i < 5; i++) { 
        cout << "I could be doing some work here" << endl; 
        sleep(10); 
        write(pipefd[1], "hello world\n", 12); 
    } 
    return 0;
}
