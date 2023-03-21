#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	// About Forks
	//Clone the calling process, creating an exact copy.
	//Return -1 for errors, 0 to the new process,
	//and the process ID of the new process to the old process.
	int id = fork();
	// Child process created

	printf("Hello World from id: %d\n",id); //Prints Hello World twice

	if(id == 0)
	{
		printf("Hello from Child Process!\n");
	}
	else{
		printf("Hello from Parent Process!\n");
	}


	return 0;
}
// Child process = 0
