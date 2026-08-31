#include <cstdio>
constexpr int N = 5000004, _N = 5000000;
int p[N];
bool np[N];
int main() {
    for (int i = 2; i <= _N; i++) {
        if (!np[i]) p[++p[0]] = i;
        for (int j = 1; j <= p[0] && i * p[j] <= _N; j++) {
            np[i * p[j]] = true;
            if (i % p[j] == 0) break;
        }
    }
    p[0] = 0, p[1] = 0;
    for (int i = 2; i <= _N; i++)
        p[i] = p[i-1] + (!np[i]);
    int tid, T, n, m;
    scanf("%d%d", &tid, &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        if (1ull * n * m > _N) puts("No");
        else puts(p[n*m] - p[n] >= n ? "Yes" : "No");
    }
}