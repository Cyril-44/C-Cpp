#include <bits/stdc++.h>
#include <pthread.h>
#include <signal.h>
unsigned long long times, seconds, last;
#define rnd() (rand() * 1. / (rand() % 10000))
#define fix(x) (x != 0 ? x : 114.514)
void* kaoji(void* arg) {
    double a = rnd(), b = rnd(), c = rnd();
    while (1) {
        c = b / fix(c);
        b = a - c + b / a * c;
        a = (a + b) * c / fix(a*b);
        times += 3;
    }
}
void gettimes(int sig) {
    ++seconds;
    putchar('|');
    register int temp = times - last;
    last = times;
    printf("Second %llu: %llu Flops\n", seconds, temp);
    alarm(1);
}
int main(int argv, char** argc) {
    srand(time(nullptr));
    unsigned thrn;
    if (argv != 2) {
        printf("Enter threads (according to your cpu threads): ");
        scanf("%2d", &thrn);
    }
    else thrn = atoi(argc[1]);
    if (thrn > 100) return 0;

    pthread_t threads[thrn];
    pthread_attr_t attr;
    sched_param param;

    // Initialize and set thread attributes
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_attr_setschedparam(&attr, &param);
    // Create threads
    for(int i = 0; i < thrn; i++) {
        int rc = pthread_create(&threads[i], &attr, kaoji, NULL);
        if (rc) {
            std::cout << "Error: Unable to create thread, " << rc << std::endl;
            exit(-1);
        }
    }
    
    signal(SIGALRM, gettimes);
    times = 0;
    alarm(1);
    while(1) pause();
    return 0;
}