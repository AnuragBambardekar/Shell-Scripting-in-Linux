#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10

// mutex for protecting the ticket variable
pthread_mutex_t ticket_mutex = PTHREAD_MUTEX_INITIALIZER;

// binary semaphore for signaling new customers
sem_t customer_sem;

// counting semaphore for limiting the number of customers served
sem_t service_sem;

// variable for the ticket number
int ticket = 0;

// function for generating a random wait time for the tax agent
int generate_wait_time() {
    return rand() % 6;
}

// function for the customer thread
void* customer(void* arg) {
    int customer_id = *(int*)arg;
    int customer_ticket;

    // get a ticket number
    pthread_mutex_lock(&ticket_mutex);
    customer_ticket = ticket;
    ticket++;
    pthread_mutex_unlock(&ticket_mutex);

    printf("Customer %d got ticket number %d\n", customer_id, customer_ticket);

    // signal the tax agent that a customer has arrived
    sem_post(&customer_sem);

    // wait to be served
    sem_wait(&service_sem);

    // simulate placing the order
    int order_time = rand() % 6;
    printf("Now Serving: %d\n", customer_ticket);
    printf("Customer %d will take %d seconds to place their order.\n", customer_id, order_time);
    sleep(order_time);

    // signal the tax agent that the customer has been served
    sem_post(&service_sem);

    printf("Customer %d has been served and is exiting.\n", customer_id);

    return NULL;
}

// function for the tax agent thread
void* tax_agent(void* arg) {
    int served_customers = 0;

    while (served_customers < NUM_CUSTOMERS) {
        // wait for a customer to arrive
        sem_wait(&customer_sem);

        // serve the customer
        int wait_time = generate_wait_time();
        printf("Tax agent will take %d seconds to serve the customer.\n", wait_time);
        sleep(wait_time);

        // signal the next customer
        served_customers++;
        sem_post(&service_sem);
    }

    printf("All customers have been served.\n");

    return NULL;
}

int main() {

    // initialize the semaphores
    sem_init(&customer_sem, 0, 0);
    sem_init(&service_sem, 0, 1);

    // create and start the tax agent thread
    pthread_t agent_thread;
    pthread_create(&agent_thread, NULL, tax_agent, NULL);

    // create and start the customer threads
    pthread_t customer_threads[NUM_CUSTOMERS];
    int customer_ids[NUM_CUSTOMERS];
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i;
        pthread_create(&customer_threads[i], NULL, customer, &customer_ids[i]);

        // wait for a random amount of time before creating the next customer thread
        int wait_time = rand() % 6;
        sleep(wait_time);
    }

    // join the customer threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    // join the tax agent thread
    pthread_join(agent_thread, NULL);

    // destroy the semaphores
    sem_destroy(&customer_sem);
    sem_destroy(&service_sem);

    return 0;
}
