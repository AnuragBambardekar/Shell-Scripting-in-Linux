#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void){
    char write_msg[BUFFER_SIZE] = "Greetings!, G'Morning";
    char read_msg[BUFFER_SIZE];    
    int fd[2];
    pid_t pid;

    //Broken PIPE Error thrown
    // Don't fork before pipe
    // pid_t pid;//Fork a child process
    // pid = fork(); 

    //Create the Pipe
    if(pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    //Fork a child process
    // Always fork after pipe
    pid = fork();

    if(pid<0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    if(pid>0){ 
        //Parent Scope
        close(fd[READ_END]);
        write(fd[WRITE_END], write_msg, strlen((write_msg)));
        printf("Writing %zd bytes\n", strlen(write_msg)); //Greetings/0 10 bytes
        close(fd[WRITE_END]);
    } else { 
        //Child Scope
        close(fd[WRITE_END]);
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("Read contents: %s\n", read_msg);
        close(fd[READ_END]);
    }
    return 0;
}