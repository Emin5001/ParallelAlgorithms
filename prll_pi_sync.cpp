#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#define MAX_THREADS 512

pthread_t handles[MAX_THREADS];
int threadArg[MAX_THREADS];
double globalSum = 0.0;
pthread_mutex_t globalSum_lock;
void *compute_pi (void *);
int numPoints;
int numThreads;
double step;

double f(double x) {
    return (6.0/sqrt(1-x*x));
}

int main(int argc, char *argv[]) {
    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_mutex_init(&globalSum_lock, NULL);
    struct timespec tick, tock;
    uint64_t execTime;
    numPoints = 100000000;
    step = 0.5/numPoints;
    pthread_mutex_init(&globalSum_lock, NULL);
    numThreads = 4;

    //create threads and initialize sum array
    for (int i=0; i< numThreads; i++) {
        threadArg[i] = i;
        pthread_create(& handles[i],&attr,compute_pi,& threadArg[i]);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

    for (int i=0; i< numThreads; i++) {
        pthread_join(handles[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("value of pi with %d threads: %f\n", numThreads, globalSum);
    return 0;
}

void *compute_pi (void *threadIdPtr) {
    int myId = *(int *)threadIdPtr;
    for (int i = myId; i < numPoints; i += numThreads) {
        double x = step * ((double) i); // next x
        double value = step*f(x);
        pthread_mutex_lock(&globalSum_lock);
        globalSum = globalSum + value; // Add to globalSum
        pthread_mutex_unlock(&globalSum_lock);
    }

    return nullptr;
}