#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

//Execution
// anuragb@anuragb:~/ECE434/pipes$ g++ 5_PipeLoop.cpp -o 5_PipeLoop
// anuragb@anuragb:~/ECE434/pipes$ ./5_PipeLoop 

// pipes with children 
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
        } 
        else if (pid == 0) { // child 
            cout << "in child with pid " << getpid() << endl; 
            int n = read(pipefd[0], buffer, 12); 
            buffer[n] = '\0'; 
            cout << "You typed '" << buffer << "'" << endl; 
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

/**
in parent with pid 6570
in child with pid 6571
in parent with pid 6570
in parent with pid 6570
in child with pid 6573
in parent with pid 6570
in child with pid 6574
in parent with pid 6570
I could be doing some work here
in child with pid 6575
in child with pid 6572
I could be doing some work here
You typed 'hello world
'
I could be doing some work here
You typed 'hello world
'
I could be doing some work here
You typed 'hello world
'
I could be doing some work here
You typed 'hello world
'
anuragb@anuragb:~/ECE434/pipes$ You typed 'hello world
*/