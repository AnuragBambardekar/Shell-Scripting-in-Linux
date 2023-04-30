#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CUSTOMERS 10
#define MAX_TICKETS 10

int ticket_num = 0; // global variable to keep track of ticket number
int next_ticket = 0; // global variable to keep track of the next available ticket
int num_customers = 0; // global variable to keep track of the number of customers
int ticket[MAX_CUSTOMERS]; // array to hold ticket numbers for each customer
int serving = -1; // global variable to keep track of the ticket being served
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex to protect access to shared variables
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // condition variable to signal when a new ticket is available

void *customer_thread(void *arg) {
    int customer_id = *((int *) arg);
    pthread_mutex_lock(&mutex); // acquire mutex

    // issue a ticket to the customer
    ticket[customer_id] = ticket_num++;
    printf("Customer %d got ticket number %d\n", customer_id, ticket[customer_id]);

    // update the next available ticket
    next_ticket++;

    // signal that a new ticket is available
    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex); // release mutex

    return NULL;
}

int generate_wait_time() {
    return rand() % 6;
}

void *tax_agent_thread_func (void *arg) {
    int i;
    pthread_mutex_lock(&mutex); // acquire mutex

    while (1) {
        // wait for a new ticket to be available
        while (next_ticket <= serving) {
            pthread_cond_wait(&cond, &mutex);
        }

        // find the lowest unserved number previously dispensed
        int lowest_unserved = -1;
        for (i = 0; i < num_customers; i++) {
            if (ticket[i] > serving && (lowest_unserved == -1 || ticket[i] < lowest_unserved)) {
                lowest_unserved = ticket[i];
            }
        }

        // serve the customer with the lowest unserved number
        serving = lowest_unserved;
        printf("Tax agent is serving customer with ticket number %d\n", serving);

        pthread_mutex_unlock(&mutex); // release mutex

        // simulate serving time
        int wait_time = generate_wait_time();
        printf("Tax agent will take %d seconds to serve the customer.\n", wait_time);
        sleep(wait_time);

        pthread_mutex_lock(&mutex); // acquire mutex

        // check if all customers have been served
        if (serving == next_ticket - 1) {
            printf("All customers have been served.\n");
            pthread_mutex_unlock(&mutex); // release mutex
            return NULL;
        }
    }
}

int main() {
    int i, customer_ids[MAX_CUSTOMERS];
    pthread_t customer_threads[MAX_CUSTOMERS], tax_agent_thread;

    // create customer threads
    for (i = 0; i < MAX_CUSTOMERS; i++) {
        customer_ids[i] = i;
        if (pthread_create(&customer_threads[i], NULL, customer_thread, &customer_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    num_customers = MAX_CUSTOMERS;

    // create tax agent thread
    if (pthread_create(&tax_agent_thread, NULL, tax_agent_thread_func, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // wait for customer threads to finish
    for (i = 0; i < MAX_CUSTOMERS; i++) {
        if (pthread_join(customer_threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    if (pthread_join(tax_agent_thread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    return 0;
}
