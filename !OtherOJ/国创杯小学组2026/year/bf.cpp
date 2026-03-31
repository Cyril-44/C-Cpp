#include <cstdio>
int main() {
    int C, t, m, a, b, c, n, cnt;
    scanf("%d%d", &C, &t);
    while (t--) {
        scanf("%d%d%d%d%d", &m, &a, &b, &c, &n);
        cnt = 0;
        for (;;m++) {
            if (m % a == 0 && (m % b != 0 || m % c == 0))
                if (++cnt == n) { break; }
        }
        printf("%d\n", m);
    }
    return 0;
}