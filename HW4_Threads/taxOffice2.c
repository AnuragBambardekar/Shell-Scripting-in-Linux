#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

int seq_num = 1;
int ticket_num = 1;
int called_num = 0;

void *sequencer(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (ticket_num <= called_num)
        {
            pthread_cond_wait(&cv, &mutex);
        }

        printf("Sequencer: Dispensing ticket number %d\n", ticket_num);
        ticket_num++;

        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *tax_agent(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (called_num >= ticket_num)
        {
            pthread_cond_wait(&cv, &mutex);
        }

        printf("Tax Agent: Calling number %d\n", called_num + 1);
        called_num++;

        pthread_cond_broadcast(&cv);

        while (called_num == ticket_num)
        {
            pthread_cond_wait(&cv, &mutex);
        }

        printf("Tax Agent: Serving customer %d\n", called_num);

        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *customer(void *arg)
{
    int my_ticket_num = 0;

    pthread_mutex_lock(&mutex);
    my_ticket_num = seq_num++;
    pthread_mutex_unlock(&mutex);

    printf("Customer %d takes ticket number %d\n", my_ticket_num, my_ticket_num);

    pthread_mutex_lock(&mutex);

    while (called_num < my_ticket_num - 1)
    {
        pthread_cond_wait(&cv, &mutex);
    }

    printf("Customer %d is now being served\n", my_ticket_num);

    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t seq_thr, ta_thr, cust_thr1, cust_thr2, cust_thr3;

    pthread_create(&seq_thr, NULL, sequencer, NULL);
    pthread_create(&cust_thr1, NULL, customer, NULL);
    pthread_create(&ta_thr, NULL, tax_agent, NULL);
    // pthread_create(&cust_thr2, NULL, customer, NULL);
    // pthread_create(&cust_thr3, NULL, customer, NULL);

    pthread_join(seq_thr, NULL);
    pthread_join(cust_thr1, NULL);
    pthread_join(ta_thr, NULL);
    // pthread_join(cust_thr2, NULL);
    // pthread_join(cust_thr3, NULL);

    return 0;
}
