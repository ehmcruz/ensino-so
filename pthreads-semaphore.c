#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS	4
#define N 10000000

volatile int count = 0;

sem_t sem;

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
		sem_wait(&sem);
		count++;
		sem_post(&sem);
	}
	
	return NULL;
}

int main (int argc, char **argv)
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	struct param_t params[NUM_THREADS];

	sem_init(&sem, 0, 1);

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

	sem_destroy(&sem);

	printf("all threads ended.\nCount = %i\n", count);

	return 0;
}