#include <random>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <vector>
#include <time.h>

double generate_random();
void*  first_prefix(void *);
void*  update_prefix(int);

const int NUM_THREADS = 4;         /* = p*/
const int ARRAY_SIZE = 1000000;    /* = n*/
const int SEGMENT_SIZE = ARRAY_SIZE / NUM_THREADS; /* = m*/

pthread_t         handles[NUM_THREADS];
pthread_t         thread_ids[NUM_THREADS];
pthread_mutex_t   lock;
pthread_barrier_t barrier;
double            array[ARRAY_SIZE]; /* = X*/

// holds the leftsum of each segment.
std::vector<double> segment_sum;

//The input array is divided into t segments if there are t threads.

//Each thread computes the prefix sum of its segment, assuming its fromleft 
//value is 0. At the end of this step, the first segment has the correct 
//prefix sum values but the other ones need a correction to account for the 
//fromleft value.

//Next, the second segment is updated with the fromleft value from the first 
//segment. This can be done in parallel by dividing the work between the t 
//threads (see the example on Slide 4). You may find it better to use fewer than 
//t threads for this step, so feel free to experiment.

// Repeat the previous step for each of the remaining segments using the 
//fromleft value from the previous segment. 

int main() {
    pthread_mutex_init(&lock, NULL);
    pthread_barrier_init (&barrier, NULL, NUM_THREADS);
    struct timespec tick, tock;
    uint64_t exectime;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = generate_random();
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&handles[i], NULL, first_prefix, &thread_ids[i]);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(handles[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &tock);

    exectime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    std::cout << "elapsed CPU time: " << exectime << " nanoseconds with " << NUM_THREADS << " threads and size of " << ARRAY_SIZE << "\n";
}

/* calculate prefix sum of each segment that is passed in. indexed into through thread_id * SEGMENT_SIZE */
void *first_prefix(void *arg) {
    int thread_id = *((int *) arg);
    int offset = thread_id * SEGMENT_SIZE;
    for (int j = 1; j < SEGMENT_SIZE; j++) {
        array[j + offset] = array[j + offset] + array[j + offset - 1];
    }
    // barrier
    pthread_barrier_wait(&barrier);
    // add each fromleft to next segment.
    update_prefix(thread_id);
    return nullptr;
}

/* update prefix of current segment with fromleft of previous segment. */
void *update_prefix(int tid) {
    for (int i = SEGMENT_SIZE; i < ARRAY_SIZE; i += SEGMENT_SIZE) {
        for (int k = tid; k < SEGMENT_SIZE; k += NUM_THREADS) {
            array[i + k] += array[i - 1];
        }
        // wait until the current segment is updated before you go onto the next one.
        pthread_barrier_wait(&barrier);
    }

    return nullptr;
}
/* helper function that just assisted in generating random numbers between 0 and 10 */
double generate_random() {
    double lower_bound = 0;
    double upper_bound = 10;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;
    return unif(re);
}
