#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 3
#define N 2
#define K 3

int A[M][N] = { {1, 4},
                {2, 5},
                {3, 6} };

int B[N][K] = { {8, 7, 6},
                {5, 4, 3} };

int C[M][K];

typedef struct {
    int i;
    int j;
} position;

void *thread_multiply(void *arg) {
    position *pos = (position *) arg;
    int i = pos->i;
    int j = pos->j;
    int sum = 0;

    for (int k = 0; k < N; k++) {
        sum += A[i][k] * B[k][j];
    }

    C[i][j] = sum;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[M][K];
    int rc;

    // Creating worker threads
    printf("Creating worker threads...\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            position *pos = malloc(sizeof(position));
            pos->i = i;
            pos->j = j;

            rc = pthread_create(&threads[i][j], NULL, thread_multiply, (void *) pos);

            if (rc) {
                printf("Error: return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
    }

    // wair for threads to output result
    printf("Joining worker threads...\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    printf("Resultant matrix:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
