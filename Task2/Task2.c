#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

float pi = 0;  // Global variable to store the calculated value of pi
float approxPi;  // Global variable to store the approximated value of pi

pthread_mutex_t mutex;  // Mutex for thread synchronization

// Structure to store thread arguments (start and finish indices)
struct thread {
    int start;
    int finish;
};

// Function executed by each thread to calculate a part of the sum for pi
void *threadFun(void *p) {
    int start;
    int finish;
    int i;

    struct thread *targs = (struct thread *)p;
    start = targs->start;
    finish = targs->finish;

    pthread_mutex_lock(&mutex);  // Lock mutex to avoid data race

    for (i = start; i < finish; i++) {
        float pi_num = pow(-1, i);
        float pi_den = ((2 * i) + 1);
        pi += (pi_num / pi_den);
    }

    pthread_mutex_unlock(&mutex);  // Unlock mutex after finishing the calculation

    pthread_exit(0);
}

int main() {
    int i;
    int threadNum, iterations;

    // Input number of iterations (i) and number of threads
    printf("Enter number of iterations (i): ");
    scanf("%d", &iterations);

    printf("Enter number of threads: ");
    scanf("%d", &threadNum);

    int chunks[threadNum];  // Array to store the number of iterations for each thread

    int rem = iterations % threadNum;

    struct thread targs[threadNum];  // Array to store thread arguments

    pthread_t thread[threadNum];  // Array to store thread IDs

    pthread_mutex_init(&mutex, NULL);  // Initialize mutex for synchronization

    // Divide iterations among threads
    for (i = 0; i < threadNum; i++) {
        chunks[i] = iterations / threadNum;
    }

    // Distribute the remaining iterations among the first 'rem' threads
    for (i = 0; i < rem; i++) {
        chunks[i] = chunks[i] + 1;
    }

    // Set start and finish indices for each thread
    for (i = 0; i < threadNum; i++) {
        if (i == 0) {
            targs[i].start = 0;
        } else {
            targs[i].start = targs[i - 1].finish + 1;
        }

        targs[i].finish = targs[i].start + chunks[i] - 1;
    }

    // Create threads and execute threadFun
    for (i = 0; i < threadNum; i++) {
        pthread_create(&thread[i], NULL, threadFun, &targs[i]);
    }

    // Wait for all threads to finish
    for (i = 0; i < threadNum; i++) {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);  // Destroy mutex after usage

    approxPi = pi * 4;  // Calculate the approximate value of pi
    printf("The appropriate value of PI is %f\n", approxPi);

    return 0;
}
