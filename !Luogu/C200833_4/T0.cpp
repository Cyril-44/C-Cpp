#include <stdio.h>
const int N = 10000000;
int main() {
    int p1, p0, f, l, scr;
    scanf("%d%d%d%d", &p1, &p0, &f, &l);
    scr = (int) ((p1 * N + p0 * N + f * N / 2) * 1. / (p1 + p0 + f + l) + p1);
    if (scr >= N / 100 * 99) puts("EX+");
    else if (scr >= N / 100 * 98) puts("EX");
    else if (scr >= N / 100 * 95) puts("AA");
    else if (scr >= N / 100 * 92) puts("A");
    else if (scr >= N / 100 * 89) puts("B");
    else if (scr >= N / 100 * 86) puts("C");
    else puts("D");
    return 0;
}