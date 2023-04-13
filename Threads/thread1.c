#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int sum; //global
// void *runner(void *param); // Function Prototype
void *runner(void *param) // void * function can take a pointer that doesn't need to be a specific type
{
    int upper = *((int *) param); //casting void *param to int *param
    sum = 0;

    int i;
    for (i=0; i<=upper; i++)
    {
        sum += i;
    }

    //send sum to main()
    // threads share everything except stacks

    pthread_exit(0);

}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: 1 integer expected.");
        return -1;
    }

    pthread_t tid;
    int param = atoi(argv[1]);
    sleep(10);

    pthread_create(&tid, NULL, runner, &param);

    pthread_join(tid, NULL);

    printf("Sum: %d\n", sum);

    return 0;
}

