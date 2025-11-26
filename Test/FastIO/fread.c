#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BILLION 1000000000L
int a[1000000];
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) { perror("fopen"); return 1; }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int64_t sum = 0;
    int32_t n;
    fscanf(fp, "%d", &n);

    int32_t x;
    for (int32_t i = 0; i < n; i++) {
        fscanf(fp, "%d", &x);
        sum += x;
    }

    fclose(fp);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / (double)BILLION;

    printf("fread/fscanf: n=%d, sum=%lld, time=%.3f sec\n",
           n, (long long)sum, elapsed);
    system("grep VmPeak /proc/$PPID/status");
    return 0;
}
