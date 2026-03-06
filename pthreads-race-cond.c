#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS	4
#define N 100000000

volatile int count = 0;

void* callback (void *data)
{
	int i, n;
	
	n = (int)data;

	for (i=0; i<n; i++)
		count++;
	
	return NULL;
}

int main (int argc, char **argv)
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	for(t=0; t<NUM_THREADS; t++) {
		printf("In main: creating thread %ld\n", t);
		
		rc = pthread_create(&threads[t], NULL, callback, (void *)N);
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