#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int i,c=0;
    for(i=0;i<4;i++)
    {
        fork();
        printf("hello\n"); //16
    }
    return 0;
}