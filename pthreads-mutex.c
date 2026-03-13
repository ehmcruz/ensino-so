#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS	4
#define N 10000000

volatile int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct param_t {
	int tid;
	int n;
};

void* callback (void *data)
{
	int i, n, tid;
	struct param_t *param = (struct param_t*)data;
	
	n = param->n;
	tid = param->tid;

	for (i=0; i<n; i++) {
		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
}

int main (int argc, char **argv)
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	struct param_t params[NUM_THREADS];

	for(t=0; t<NUM_THREADS; t++) {
		printf("In main: creating thread %ld\n", t);

		params[t].tid = t;
		params[t].n = N;
		
		rc = pthread_create(&threads[t], NULL, callback, (void *)&params[t]);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for(t=0; t<NUM_THREADS; t++)
		pthread_join(threads[t], NULL);

	printf("all threads ended.\nCount = %i\n", count);

	return 0;
}