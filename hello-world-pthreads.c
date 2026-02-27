#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS	5
#define N 10

void *PrintHello (void *threadid)
{
	long tid;
	int i;
	
	tid = (long)threadid;

	for (i=0; i<N; i++) {
		printf("Hello World! It's me, thread #%ld!\n", tid);
		usleep(500000);
	}
	
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	for(t=0;t<NUM_THREADS;t++) {
		printf("In main: creating thread %ld\n", t);
		
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for(t=0;t<NUM_THREADS;t++)
		pthread_join(threads[t], NULL);

	printf("all threads ended\n");

	return 0;
}