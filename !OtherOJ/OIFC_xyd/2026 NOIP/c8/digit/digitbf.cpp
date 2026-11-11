#include <bits/stdc++.h>
constexpr int N = 105;
int f[N];
inline int pow2(int x) { return x * x; }
inline int calc(int64_t x) {
    int res = 0;
    for (; x; x /= 10) res += pow2(x%10);
    return res;
}
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%d", &f[i]);
    while (q--) {
        int op,l , r; scanf("%d%d%d", &op, &l, &r);
        if (op == 1) {
            for (int i = l; i <= r; i++) f[i] = calc(f[i]);
        } else {
            int res = 0;
            for (int i = l; i <= r; i++) res += f[i];
            printf("%d\n", res);
        }
    }
}