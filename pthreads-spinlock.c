#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS	4
#define N 10000000

volatile int count = 0;

typedef int spinlock_t;

#if defined(__x86_64__)
	#define CPU_PAUSE __asm__ __volatile__ ("pause");
#else
	#define CPU_PAUSE
#endif

#define SPINLOCK_INIT 0

spinlock_t lock = SPINLOCK_INIT;

static inline void spinlock_lock (spinlock_t *exclusion)
{
	while (__sync_lock_test_and_set(exclusion, 1)) {
		CPU_PAUSE
	}
}

static inline void spinlock_unlock (spinlock_t *exclusion)
{
	__sync_synchronize(); // Memory barrier.
	__sync_lock_release(exclusion);
}

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
		spinlock_lock(&lock);
		count++;
		spinlock_unlock(&lock);
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