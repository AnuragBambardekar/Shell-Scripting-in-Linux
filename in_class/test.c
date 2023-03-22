#include  <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <features.h> 


void main() {
    printf("%d",getppid());
    int p1 = fork();

    if (p1 == 0) {

        execlp("/bin/ls","ls",NULL); // ECE434Quiz3 is a different program that does not print to console

        printf("Step1: %d, %d    ", getpid(), getppid());

     }

    else {

        // wait(p1);

        int p2 = fork();

        if (p2 == 0) {

            printf("Step2: %d, %d    ", getpid(), getppid());

        } else {

            wait(p2);

            printf("Step3: %d, %d     ", getpid(), getppid());

        }

     }

 }