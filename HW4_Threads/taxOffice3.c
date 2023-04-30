#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int ticket_seq = 0;
int ticket_served = 0;

void *sequencer(void *arg) {
    int *tickets = (int*) arg;
    int i;
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_mutex_lock(&mutex);
        printf("Issuing ticket to customer: %d\n",i);
        tickets[i] = ticket_seq++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *customer(void *arg) {
    int *ticket = (int*) arg;
    pthread_mutex_lock(&mutex);
    while (*ticket != ticket_served) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Customer with ticket number %d is being served\n\n", *ticket);
    ticket_served++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


void *tax_agent(void *arg) {
    int i;
    int *tickets = (int*) arg;
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_mutex_lock(&mutex);
        while (tickets[i] != ticket_served) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Now Serving: Ticket Number: %d\n",tickets[i]);

        printf("Tax agent is serving customer with ticket number %d\n", tickets[i]);
        pthread_cond_signal(&cond);

        // Wait for customer to place an order
        printf("Waiting for customer with ticket number %d to place an order...\n", tickets[i]);
        pthread_cond_wait(&cond, &mutex);

        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


int main() {
    pthread_t sequencer_thread, tax_agent_thread, customer_threads[NUM_CUSTOMERS];
    int tickets[NUM_CUSTOMERS];
    int i;

    pthread_create(&sequencer_thread, NULL, sequencer, tickets);
    pthread_join(sequencer_thread, NULL);

    pthread_create(&tax_agent_thread, NULL, tax_agent, tickets);

    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&customer_threads[i], NULL, customer, &tickets[i]);
    }

    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    pthread_join(tax_agent_thread, NULL);

    return 0;
}


