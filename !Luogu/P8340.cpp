#include <stdio.h>
#include <algorithm>
constexpr int N = 5005;
int f[N], M;
inline void add(int &x, int y) { if ((x += y) >= M) x -= M; }
int main() {
    int n;
    scanf("%d%d", &n, &M);
    f[1] = f[3] = 1;
    for (int i = 3; i <= n; i++)
        for (int k = n; k >= i - 1; k--)
            add(f[std::min(i + k, n)], f[k]);
    printf("%d\n", f[n]);
    return 0;
}