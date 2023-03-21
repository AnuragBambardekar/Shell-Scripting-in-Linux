#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    // 3 processes
	int id = fork();
    if(id != 0)
    {
        fork();
    }

	printf("Hello World\n");

	return 0;
}
