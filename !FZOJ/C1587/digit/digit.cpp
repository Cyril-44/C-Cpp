#include <cstdio>
template<class T>
inline void out(T x, int k) {
    static int st[40];
    for (int i = 0; i < k; i++)
        st[i] = x % 10, x /= 10;
    for (int i = k-1; i >= 0; i--)
        putchar(st[i] | '0');
    putchar('\n');
}
inline void bf(int n, int k) {
    int cnt2 = 0, cnt5 = 0;
    unsigned __int128 res = 1, Mod = 1;
    for (int i = 0; i < k; i++) Mod *= 10;
    for (int i = 2; i <= n; i++) {
        int v = i;
        while (v % 2 == 0) ++cnt2, v /= 2;
        while (v % 5 == 0) ++cnt5, v /= 5;
        res = res * v % Mod;
    }
    cnt2 -= cnt5;
    for (int i = 0; i < cnt2; i++) res = res * 2 % Mod;
    out(res, k);
}
int main() {
    int T, k;
    long long n;
    scanf("%d", &T);
    while (T--) {
        scanf("%lld%d", &n, &k);
        if (n <= 10000) bf(n, k);
    }
    return 0;
}