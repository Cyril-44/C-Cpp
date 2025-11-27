#include <pthread.h>
#include <sched.h>
#include <iostream>

#define NUM_THREADS 100

void* func(void* arg) {
    // Your code here
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    sched_param param;

    // Initialize and set thread attributes
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_attr_setschedparam(&attr, &param);

    // Create threads
    for(int i = 0; i < NUM_THREADS; i++) {
        int rc = pthread_create(&threads[i], &attr, func, NULL);
        if (rc) {
            std::cout << "Error: Unable to create thread, " << rc << std::endl;
            exit(-1);
        }
    }

    // Clean up
    pthread_attr_destroy(&attr);
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}