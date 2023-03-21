#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("PID of main.c: %d\n", getpid());
    char *args[] = {"Hello", "World!", NULL};
    execv("./6_execv_demo_sub", args); //compile sub file before

    //exec() replaces the entire process, but PID remains the same
    printf("Back to main.c");
    return 0;
}