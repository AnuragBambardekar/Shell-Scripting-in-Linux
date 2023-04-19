#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


long long *fib = NULL;
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
	pthread_create(&tid, NULL, fib_calc, &n);
	// print_fib(n); // here answer will be 0 0 0 0 0
	pthread_join(tid, NULL);
	printf("Child thread terminated.\n");

	print_fib(n);
	return 0;
}

void *fib_calc(void *terms) {
	printf("Child thread started...\n");
	int n = *((int *)terms);

	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i < n; ++i) {
		fib[i] = fib[i - 1] + fib[i - 2];
	}
	printf("Fibonacci sequence computed by child thread.\n");
	pthread_exit(0);
}

void print_fib(int n) {
	printf("Fibonacci sequence (%d terms): \n", n);
	for (int i = 0; i < n; ++i)
		printf("%lld ", fib[i]);
	printf("\n");
}