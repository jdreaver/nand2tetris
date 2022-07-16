#include "atomic_counter.h"
#include "nonatomic_counter.h"

#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct nonatomic_thread_args {
	struct nonatomic_counter *counter;
	uint64_t count_max;
};

static void *nonatomic_thread_handler(void *arg)
{
	struct nonatomic_thread_args *thread_args = (struct nonatomic_thread_args *) arg;
	for (uint64_t i = 0; i < thread_args->count_max; i++)
		nonatomic_counter_increment(thread_args->counter);
	return NULL;
}

struct atomic_thread_args {
	struct atomic_counter *counter;
	uint64_t count_max;
};

static void *atomic_thread_handler(void *arg)
{
	struct atomic_thread_args *thread_args = (struct atomic_thread_args *) arg;
	for (uint64_t i = 0; i < thread_args->count_max; i++)
		atomic_counter_increment(thread_args->counter);
	return NULL;
}

int main()
{
	// TODO: Make these command line args.
	uint16_t num_threads = 4;
	uint64_t count_max = 1000000;

	struct timespec start, finish;
	double elapsed_millis;

	// Set up threads
	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

	// Non-atomic counter. This is here just to show that without locks the
	// threads step on one another and we get an inaccurate count.
	struct nonatomic_counter *nonatomic_counter = nonatomic_counter_create();
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (size_t i = 0; i < num_threads; i++) {
		struct nonatomic_thread_args arg = {
			.counter = nonatomic_counter,
			.count_max = count_max / num_threads,
		};
		if (pthread_create(&threads[i], NULL, nonatomic_thread_handler, &arg)) {
			fprintf(stderr, "error creating nonatomic counter pthread\n");
			exit(EXIT_FAILURE);
		}
	}
	for (size_t i = 0; i < num_threads; i++)
		pthread_join(threads[i], NULL);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed_millis = (finish.tv_sec - start.tv_sec);
	elapsed_millis += (finish.tv_nsec - start.tv_nsec) / 1000000.0;

	printf("nonatomic_counter time spent (ms): %f\n", elapsed_millis);
	printf("nonatomic counter final value, expected: %" PRIu64
	       ", got: %" PRIu64 "\n",
	       count_max, nonatomic_counter_get(nonatomic_counter));

	nonatomic_counter_destroy(nonatomic_counter);

	// Atomic counter
	struct atomic_counter *atomic_counter = atomic_counter_create();
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (size_t i = 0; i < num_threads; i++) {
		struct atomic_thread_args arg = {
			.counter = atomic_counter,
			.count_max = count_max / num_threads,
		};
		if (pthread_create(&threads[i], NULL, atomic_thread_handler, &arg)) {
			fprintf(stderr, "error creating atomic counter pthread\n");
			exit(EXIT_FAILURE);
		}
	}
	for (size_t i = 0; i < num_threads; i++)
		pthread_join(threads[i], NULL);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed_millis = (finish.tv_sec - start.tv_sec);
	elapsed_millis += (finish.tv_nsec - start.tv_nsec) / 1000000.0;

	printf("atomic_counter time spent (ms): %f\n", elapsed_millis);
	printf("atomic counter final value, expected: %" PRIu64
	       ", got: %" PRIu64 "\n",
	       count_max, atomic_counter_get(atomic_counter));

	atomic_counter_destroy(atomic_counter);

	free(threads);
}
