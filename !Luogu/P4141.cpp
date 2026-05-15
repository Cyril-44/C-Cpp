#include <cstdio>
constexpr int N = 2005, MOD = 10;
int f[N], w[N];
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline void sub(int &x, int y) { if ((x -= y) < 0) x += MOD; }
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &w[i]);
        for (int j = m; j >= w[i]; j--) add(f[j], f[j - w[i]]);
    }
    for (int i = 1; i <= n; i++) {
        for (int j = w[i]; j <= m; j++) sub(f[j], f[j - w[i]]);
        for (int j = 1; j <= m; j++) putchar(f[j] | '0');
        putchar('\n');
        for (int j = m; j >= w[i]; j--) add(f[j], f[j - w[i]]);
    }
    return 0;
}