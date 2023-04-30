#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10
#define TICKET_INITIAL_VALUE 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex to protect the shared data
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER; // condition variable to signal changes to the shared data

int next_ticket = TICKET_INITIAL_VALUE; // the next available ticket number
int called_ticket = TICKET_INITIAL_VALUE; // the number of the ticket that was called by the tax agent
int tickets[NUM_CUSTOMERS]; // array of customer tickets

void* sequencer_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex); // lock the mutex
        tickets[next_ticket - 1] = next_ticket++; // issue a new ticket
        pthread_mutex_unlock(&mutex); // unlock the mutex
        pthread_cond_broadcast(&cond_var); // notify all waiting threads that new tickets are available
        usleep(500000); // wait for 0.5 seconds
    }
    return NULL;
}

void* customer_thread(void* arg) {
    int id = *(int*)arg;
    pthread_mutex_lock(&mutex); // lock the mutex
    int my_ticket = tickets[id - 1]; // take the next available ticket
    while (my_ticket > called_ticket) { // wait until my ticket is called
        pthread_cond_wait(&cond_var, &mutex); // release the lock and wait for the condition variable to be signaled
    }
    printf("Customer %d is being served (ticket #%d)\n", id, my_ticket);
    pthread_mutex_unlock(&mutex); // unlock the mutex
    usleep(1000000); // simulate the time it takes for the customer to be served
    return NULL;
}

void* tax_agent_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex); // lock the mutex
        while (called_ticket >= next_ticket) { // wait until there are unserved tickets
            pthread_cond_wait(&cond_var, &mutex); // release the lock and wait for the condition variable to be signaled
        }
        printf("Tax agent is serving customer with ticket #%d\n", called_ticket);
        called_ticket++; // move to the next ticket
        pthread_mutex_unlock(&mutex); // unlock the mutex
        usleep(1000000); // simulate the time it takes for the tax agent to serve the customer
    }
    return NULL;
}

int main() {
    pthread_t sequencer;
    pthread_t tax_agent;
    pthread_t customers[NUM_CUSTOMERS];
    int customer_ids[NUM_CUSTOMERS];

    pthread_create(&sequencer, NULL, sequencer_thread, NULL);
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, customer_thread, &customer_ids[i]);
    }
    pthread_create(&tax_agent, NULL, tax_agent_thread, NULL);

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    pthread_join(sequencer, NULL);
    pthread_join(tax_agent, NULL);

    return 0;
}
