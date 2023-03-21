#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 64


//Create a function for printing
void print_parent(int x)
{
    // char cmd[200];
    printf("Parent: I am process %d and k is %d\n", (int)getpid(),x);
    // pid_t pid_real = getpid();

    // sprintf(cmd,"pstree -np -C age %d", pid_real);
    
    // int res = system(cmd);
}

void print_child(int x)
{
    // char cmd[200];
    printf("Child: I am process %d and k is %d\n", (int)getpid(),x);
    // pid_t pid_real = getpid();

    // sprintf(cmd,"pstree -np -C age %d", pid_real);
    
    // int res = system(cmd);
}

int main(int argc, char *argv[]) {
    int i, status;
    pid_t pid, child_pid;
    int *k_ptr;
    int shmid;
    key_t key;
    pid = getpid();
    printf("The initial process pid is=%d\n", pid);

    // create a shared memory segment for the k variable
    key = ftok(".", 'A'); //generates a unique key based on the current working directory (".") and the character 'A'. The key is used to create a shared memory segment using the shmget() function.
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    k_ptr = (int *) shmat(shmid, NULL, 0);
    if (k_ptr == (int *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    *k_ptr = 1;

    for (i = 0; i < 4; i++) {
        int f = fork();
        if (f == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (f != 0){
            // parent process
            //printf("Parent: I am process %d and k is %d\n", (int)pid,*k_ptr);
            //print_parent(*k_ptr);
            //printf("k: %d\n",*k_ptr);
            waitpid(f, &status, 0);
            //(*k_ptr)++;
        } else {
            // child process
            //pid = getpid();
            //(*k_ptr)++;
            //printf("Child: I am process %d and k is %d\n", (int)getpid(),*k_ptr);
            printf("I am process %d and k is %d\n", (int)getpid(),*k_ptr);
            //print_child(*k_ptr);
            //printf("k: %d\n",*k_ptr);
            (*k_ptr)++;
            // exit(EXIT_SUCCESS);
            
        }
    }

    // detach and remove shared memory segment
    // if (shmdt(k_ptr) == -1) {
    //     perror("shmdt");
    //     exit(EXIT_FAILURE);
    // }
    // if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    //     perror("shmctl");
    //     exit(EXIT_FAILURE);
    // }

    // printf("Hello \n\n");
    char cmd[200];

    sprintf(cmd,"pstree -np -C age %d", pid);
    
    int res = system(cmd);

    return 0;
}
