#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("In new_file\n");
    printf("PID of new_file.c: %d\n", getpid());
    
    return 0;
}