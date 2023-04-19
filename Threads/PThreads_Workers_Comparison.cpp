/**
 * Measure task number for each thread (num_tasks_performed) until total task 
number (thread_data-> next_task) reaches maximum thread number. 
 * Every worker takes the next available number of task in the queue, add it to 
their sum, and then the next ... and next ... etc etc until the pile is empty.
 * Compare speed of run_test1 with run_test2 to determine if fewer or more 
workers allocated to a number of tasks gives better performance.

*/



// run_test1
// we generate ass many Pthreads to handle tasks 
#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <algorithm>
#include <atomic>
#include "CycleTimer.h"
#define NUM_WORKER_THREADS 4

void* worker1(void* arg){
    // do nothing
    return NULL;
}

void run_test1(int num_tasks) {
    pthread_t thread[num_tasks];
    printf("Spawining 1 thread per task (%d task)\n",num_tasks);

    double min_time = -10e10;
    double max_time = 10e10;
    double start_time = CycleTimer::currentSeconds();
    double iter_start_time;

    for(int i=0; i<num_tasks; i++)
    {
        iter_start_time = CycleTimer::currentSeconds();
        pthread_create(&thread[i], NULL, worker1, NULL);
    }

    for(int i=0; i<num_tasks; i++)
    {
        pthread_join(thread[i], NULL);

        double iter_end_time = CycleTimer::currentSeconds();
        double elapsed = iter_end_time - iter_start_time;
        min_time = std::min(elapsed, min_time);
        max_time = std::max(elapsed, max_time);
    }

    double end_time = CycleTimer::currentSeconds();
    double elapsed = end_time - start_time;

    printf("Total Time: %.2f sec\n", elapsed);
    printf("Time/spawn: %.4f ms (min: %.4f ms, max: %.4f ms\n)", 1000.f *elapsed/num_tasks, 1000.f *min_time, 1000.f *max_time);
    printf("Spawns/sec: %.2f\n\n", static_cast<double>(num_tasks)/elapsed);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// run_test2
// generate only NUM_WORKER_THREADS < number of tasks
struct worker_info {
    int num_tasks;
    std::atomic<int> next_task;
};

void* worker2(void* arg) { //Will run fn workers with the address of the args main sent
    worker_info* thread_data = static_cast<worker_info*>(arg); //always do this casting
    int num_tasks_performed = 0; //thread local variable
    while (thread_data->next_task < thread_data->num_tasks) { //shared variables
        thread_data->next_task++; // shared variable
        num_tasks_performed++; // local variable
    }
    
    printf("Num tasks performed by worker thread: %d\n", num_tasks_performed);
    return NULL;
}

void run_test2(int num_tasks) {
    worker_info thread_data; // initializes arg each thread will need to run fn worker2
    thread_data.num_tasks = num_tasks; //shared var: but how does it get visible to threads?
    thread_data.next_task = 0; //shared var: but how does it get visible to threads?
    pthread_t thread[NUM_WORKER_THREADS]; // set an array of four threads
    
    double start_time = CycleTimer::currentSeconds();
    
    for (int i=0; i<NUM_WORKER_THREADS; i++) { //every thread runs worker2 with args: thread_data
        pthread_create(&thread[i], NULL, worker2, (void *)&thread_data); 
    } //threads only run what the main process sends, so need the address of the function, and address of the args

    for (int i=0; i<NUM_WORKER_THREADS; i++) pthread_join(thread[i], NULL);
    
    double end_time = CycleTimer::currentSeconds();
    double elapsed = end_time - start_time;
    
    printf("Total time: %.2f sec\n", elapsed);
    printf("Time per spawn: %.6f ms\n",1000.f * elapsed / num_tasks);
    printf("Spawns/sec: %.2f\n\n", static_cast<double>(num_tasks) / elapsed);
}


int main(int argc, char** argv) {
    int num_tasks = 1000;
    if (argc >1) {
        num_tasks = atoi(argv[1]);
    }
    run_test1(num_tasks);
    run_test2(num_tasks);
    return 0;
}

