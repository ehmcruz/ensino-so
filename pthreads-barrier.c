#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS	4

pthread_barrier_t barrier;

struct param_t {
	int tid;
	int n;
};

void* callback (void *data)
{
	int i, tid;
	struct param_t *param = (struct param_t*)data;
	
	tid = param->tid;

	printf("hello world from thread %u\n", tid);
	pthread_barrier_wait(&barrier);
	printf("bye bye from thread %u\n", tid);
	
	return NULL;
}

int main (int argc, char **argv)
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	struct param_t params[NUM_THREADS];

	pthread_barrier_init(&barrier, NULL, NUM_THREADS);

	for(t=0; t<NUM_THREADS; t++) {
		printf("In main: creating thread %ld\n", t);

		params[t].tid = t;
		
		rc = pthread_create(&threads[t], NULL, callback, (void *)&params[t]);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for(t=0; t<NUM_THREADS; t++)
		pthread_join(threads[t], NULL);

	printf("all threads ended\n");

	return 0;
}