#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char ** argv) {
    fork();

    int pid = fork();

    if (pid == 0) {
       fork();
       fork();
    }

    printf("bro\n");

    exit(0);
}