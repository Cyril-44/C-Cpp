#include <stdio.h>
#include <string.h>
#include <functional>
#include <algorithm>
#include <iostream>
#include <bitset>
constexpr int N = 5005, MOD = 998244353;
inline void add(int &x, int y) {
    // fprintf(stderr, "%d += %d\n", x, y);
    if ((x += y) >= MOD) x -= MOD;
}
inline void sub(int &x, int y) {
    // fprintf(stderr, "%d -= %d\n", x, y);
    if ((x -= y) < 0) x += MOD;
}
struct Node {
    int a, w;
    inline bool operator< (const Node &rhs) const {
        int tp1 = a * (w ^ 3), tp2 = rhs.a * (rhs.w ^ 3);
        if (tp1 == tp2) return a < rhs.a;
        return tp1 < tp2;
    }
    inline bool operator> (const Node &rhs) const { return rhs < *this; }
} god[N << 1];
int a[N];
inline int qpow(int b, int n){
    int res = 1;
    while (n) {
        if (n & 1) res = 1ll * res * b % MOD;
        b = 1ll * b * b % MOD;
        n >>= 1;
    }
    return res;
}
inline void read(int& n, int& m) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::sort(a+1, a+1 + n, std::greater<int>());
}
inline void solvebf() {
    int n, m;
    read(n, m);
    int ans = 0;
    for (unsigned st = 0; st < (1 << n); ++st) {
        Node b[40]{};
        for (int i = 0; i < n; i++)
            b[i+1] = {a[i+1], (int)((st >> i & 1) + 1)};
        std::sort(b+1, b+1 + n, std::greater<Node>());
        long long greedy = 0;
        for (int i = 1, mm = m; i <= n; i++) {
            if (mm >= b[i].w) {
                greedy += b[i].a, mm -= b[i].w;
                // fprintf(stderr, "Chosen %d, %d\n", b[i].a, b[i].w);
            }
        }
        long long f[85]{};
        memset(f, -0x3f, sizeof(long long) * 85);
        f[0] = 0;
        for (int i = 1; i <= n; i++)
            for (int j = m; j >= b[i].w; j--)
                f[j] = std::max(f[j], f[j - b[i].w] + b[i].a);
        long long mx = *std::max_element(f, f + m + 1);
        if (mx == greedy) ++ans;
#ifdef DEBUG
        std::cerr << std::bitset<5>(st);
        fprintf(stderr, " %lld %lld %s\n", greedy, mx, mx == greedy ? "OK" : "");
#endif
    }
    printf("%d\n", ans);
}
inline void solve() {
    int n, m;
    read(n, m);
}
inline void solve2() {
    // fprintf(stderr, "------\n");
    int n, m;
    read(n, m);
    int ans = qpow(2, n-1); // 如果 w1=1, 肯定是对的
    add(ans, 1);
    a[n + 1] = 0;
    bool flag = true;
    for (int i = 2; i <= n; i++) { // w1..i-1 都取 2 , wi=1
        // if (a[i] * 2 >= a[1] && a[i] + a[i+1] < a[1]) // 肯定不行
        //     continue;
        if (a[i] + a[i+1] <= a[1]) {
            if (flag && a[i] * 2 > a[1]) { flag = false; continue; }
            // fprintf(stderr, "!!!! %d %d %d %d\n", i, a[i], a[i+1], a[1]);
            int tp = qpow(2, n - i);
            add(ans, tp);
            continue;
        }
        int mx = std::upper_bound(a+1+i, a+1 + n, a[1] - a[i], std::greater<int>()) - a-1;
        // fprintf(stderr, "[%d,%d]\n", i, mx);
        int tp = qpow(2, n - i); // 后面所有可能方案数
        sub(tp, qpow(2, n - mx)); // 只要有一个 w=1 就不行
        add(ans, tp);
    }
    printf("%d\n", ans);
}
int main() {
    // freopen("sale.in", "r", stdin);
    // freopen("sale.out", "w", stdout);
    int c, t;
    scanf("%d%d", &c, &t);
    while (t--) {
        switch (c) {
        case 1:  case 2:  case 3: case 4: case 5:
            solvebf(); break;
        case 14: case 15: case 7: case 8: case 9:
            solve2(); break;
	    case 16: case 18: {
            int n, m;
            read(n, m);
            printf("%d\n", qpow(2, n));
            break;
        }
        default:
            solve();
        }
    }
    return 0;
}
