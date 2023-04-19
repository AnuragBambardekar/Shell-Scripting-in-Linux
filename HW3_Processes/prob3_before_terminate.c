#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

long long *fib = NULL;
int fib_computed = 0;

void *fib_calc(void*);
void print_fib(int);

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		printf("Usage: %s [terms]\n", argv[0]);
		return 1;
	}

	int n = atoi(argv[1]);
	if (n <= 0) {
		printf("Please enter a positive integer\n");
		return 1;
	}

	fib = malloc((n + 1) * sizeof(long long));

	pthread_t tid;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid, NULL, fib_calc, &n);

	pthread_mutex_lock(&mutex);
	while (!fib_computed) {
		printf("Parent thread waiting for signal...\n");
		pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	print_fib(n); // We have got the result before the child terminates
	pthread_join(tid, NULL);
	printf("Child thread terminated.\n");
	return 0;
}

void *fib_calc(void *terms) {
	pthread_mutex_lock(&mutex);
	printf("Child thread started...\n");

	int n = *((int *)terms);

	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i < n; ++i) {
		fib[i] = fib[i - 1] + fib[i - 2];
	}

	printf("Fibonacci sequence computed by child thread.\n");
	fib_computed = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);

	pthread_exit(0);
}

void print_fib(int n) {
	printf("Fibonacci sequence (%d terms): \n", n);
	for (int i = 0; i < n; ++i)
		printf("%lld ", fib[i]);
	printf("\n");
}
