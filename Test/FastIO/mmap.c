#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define BILLION 1000000000L

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    fstat(fd, &st);
    size_t filesize = st.st_size;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    char *data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    int64_t sum = 0;
    int32_t n = 0;
    size_t i = 0;

    // 解析第一个数 n
    while (i < filesize && isspace(data[i])) i++;
    while (i < filesize && isdigit(data[i])) {
        n = n * 10 + (data[i] - '0');
        i++;
    }

    // 解析后续 n 个数
    for (int32_t cnt = 0; cnt < n; cnt++) {
        while (i < filesize && isspace(data[i])) i++;
        int32_t val = 0, sign = 1;
        if (i < filesize && data[i] == '-') { sign = -1; i++; }
        while (i < filesize && isdigit(data[i])) {
            val = val * 10 + (data[i] - '0');
            i++;
        }
        sum += sign * val;
    }

    munmap(data, filesize);
    close(fd);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / (double)BILLION;

    printf("mmap: n=%d, sum=%lld, time=%.3f sec\n",
           n, (long long)sum, elapsed);

    return 0;
}
