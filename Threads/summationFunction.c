#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; // Shared by threads
void *runner(void *param); // threads call this function

int main(int argc, char *argv[])
{
    pthread_t tid; // thread identifier
    pthread_attr_t attr; // set of thread attributes

    if(argc != 2)
    {
        fprintf(stderr,"Usage: a.out <integer_value>\n");
        return -1;
    }
    if(atoi(argv[1]) < 0)
    {
        fprintf(stderr,"%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    // get the default attributes
    pthread_attr_init(&attr);

    // create the thread
    pthread_create(&tid, &attr, runner, argv[1]);

    // wait for thread to exit
    pthread_join(tid, NULL);

    printf("sum = %d\n", sum);
}

void *runner(void *param)
{
    int i, upper = atoi(param);
    sum = 0;

    for(i=1; i<=upper; i++)
        sum+=i;

    pthread_exit(0);
}