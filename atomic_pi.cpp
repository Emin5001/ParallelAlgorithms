#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <atomic>
#include <math.h>
 
#define NUM_THREADS 8

double f (double); 
void   *compute_pi(void *);
void   add_to_pi(double);

pthread_t           handles[NUM_THREADS];
int                 threadArg[NUM_THREADS];
std::atomic<double> pi{0.0};
int                 numPoints;
double              step;

int main() {
    uint64_t execTime; /* time in nanoseconds */
    struct timespec tick, tock;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    numPoints = 100000000;
    step = 0.5 / numPoints;
    // create threads & init sum array
    for (int i = 0; i < NUM_THREADS; i++) {
        threadArg[i] = i;
        pthread_create(&handles[i], &attr, compute_pi, &threadArg[i]);
    }
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

    // join with threads and add contr from sum arr
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(handles[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &tock);

    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("value of pi with %d threads: %f\n", NUM_THREADS, pi.load());

    return 0;
}

double f(double x) {
    return (6.0 / sqrt(1 - x * x));
} 
 
void *compute_pi(void *threadIdPtr) {
    int myId = * (int *) threadIdPtr;
    
    for (int i = myId; i < numPoints; i += NUM_THREADS) {
        double x = step * ((double) i); //next x
        double value = step * f(x);
        add_to_pi(value);
    } 

    return nullptr;
}

void add_to_pi(double bar) {
    auto current = pi.load();
    while (!pi.compare_exchange_weak(current, current + bar));
}
 
