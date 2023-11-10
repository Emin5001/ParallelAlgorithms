#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h> 
#include <time.h>   
#include <atomic>    

#define NUM_THREADS 8 

double f(double x) {
  return (6.0 / sqrt(1 - x * x));
}

double pi;
double sum[NUM_THREADS];
pthread_barrier_t barrier;
int numPoints;
double step;

void* compute_pi(void* threadIdPtr) {
    int thread_id = *(int*) threadIdPtr;
    double mySum = 0.0;
    for (int i = thread_id; i < numPoints; i += NUM_THREADS) {
      double x = step * ((double) i);
      mySum = mySum + step * f(x);
    }

    sum[thread_id] = mySum;

    pthread_barrier_wait(&barrier);
    
    return nullptr;
}

int main(int argc, char* argv[]) {
    uint64_t execTime;
    struct timespec tick, tock;
    pi = 0.0;
    numPoints = 100000000;
    step = 0.5 / numPoints;
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        sum[i] = 0.0;
        pthread_create(&threads[i], NULL, compute_pi, &thread_ids[i]);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &tick);
    pthread_barrier_wait(&barrier);
    for (int i = 0; i < NUM_THREADS; i++) {
        pi += sum[i];
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("Number of Threads: %d\n", NUM_THREADS);
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int)execTime);
    printf("%.6f\n", pi);
    return 0;
}