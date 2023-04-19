#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Compute Square of Value
int value; // Thread stores result here

void *my_thread(void *param);

int main(int argc, char *argv[]){
    pthread_t tid;
    int retcode;

    if(argc != 2){
        fprintf(stderr, "Usage: a.out <integer_value>\n");
        exit(0);
    }

    // Create the thread
    retcode = pthread_create(&tid, NULL, my_thread, argv[1]);
    if(retcode != 0) {
        fprintf(stderr, "Unable to create Thread,\n");
        exit(1);
    }

    pthread_join(tid, NULL);
    printf("Parent: Square = %d\n", value);
    return 0;
}

// Wait for created thread to exit
void *my_thread(void *param) {
    int i = atoi(param);

    printf("I am the child, passed value: %d\n", i);
    value = i*i;

    pthread_exit(0);
}