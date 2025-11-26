#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    srand(time(NULL));
    freopen("test.in", "w", stdout);
    int n = 1e8, i, ai;
    printf("%d ", n);
    long long sum = 0;
    fprintf(stderr, "Tot %d\n", n >> 18);
    for (i = 1; i <= n; i++) {
        ai = rand();
        printf("%d ", ai);
        sum += ai;
        if (!(i & 0x3ffff)) fprintf(stderr, "Get %d\n", i >> 18);
    }
    fclose(stdout);
    fprintf(stderr, "%lld\n", sum);
    return 0;
}