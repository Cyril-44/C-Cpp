#include <bits/stdc++.h>
constexpr int N = 200005, B = 154, MOD = (int)1e9 + 9;
int a[N], b[N], c[N];
unsigned base[N];
std::bitset<N> on;
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FastI() : p1(), p2() {}
    [[gnu::always_inline]] inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void operator()(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} fin;
inline unsigned calcHash(int *a, int n) {
    unsigned ans = 0;
    for (int i = 1; i <= n; i++)
        if ((ans += 1ull * a[i] * base[i-1] % MOD) >= MOD)
            ans -= MOD;
    return ans;
}
void calBF(int n, int m) {
    std::vector<int> v1, v2;
    for (int i = 1; i <= n; i++) {
        v1.push_back(a[i]);
        v2.push_back(a[i]);
        if (on[i]) {
            std::vector<int> t1(v1), t2(v2);
            reverse(t1.begin(), t1.end());
            reverse(t2.begin(), t2.end());
            for (int j = 0; j < i; j++) {
                if (v1[j] > t2[j]) { v1 = t2; break; }
                else if (v1[j] < t2[j]) break;
            }
            for (int j = i - 1; j >= 0; j--) {
                if (v2[j] > t1[j]) { v2 = t1; break; }
                else if (v2[j] < t1[j]) break;
            }
        }
    }
    for (int i = n; i > 0; i--) a[i] = v1.back(), v1.pop_back();
    printf("%d\n", calcHash(a, n));
}
inline void solveSingle() {
    int n, m;
    fin(n), fin(m);
    on.set();
    for (int i = 1; i <= n; i++) fin(a[i]);
    for (int i = 1, bi; i <= m; i++) {
        fin(bi);
        on.reset(bi);
    }
    if (n <= 2000) return calBF(n, m);

    static int _a[N], premn[N];
    memcpy(_a, a, sizeof(int) * (n+1));
    std::sort(_a+1, _a+1+n);
    _a[0] = std::unique(_a+1, _a+1+n)-_a;
    for (int i = 1; i <= n; i++) {
        a[i] = std::lower_bound(_a+1, _a+*_a, a[i]) - _a;
        premn[i] = i == 1 ? a[i] : premn[i-1];
        if (on[i]) premn[i] = std::min(premn[i], a[i]);
    }
    
    b[0] = c[0] = 0;
    bool state = false;
    for (int i = n; i > 0; i--) {
        if ((on[i] || state) && a[i] == premn[i]) {
            b[++b[0]] = a[i];
            state = true;
        }
        else c[++c[0]] = a[i], state = false;
    }
    memcpy(a+1, b+1, sizeof(int) * b[0]);
    std::reverse(c+1, c+1+c[0]);
    memcpy(a+b[0]+1, c+1, sizeof(int) * c[0]);
    // for (int i = 1; i <= n; i++) printf("%d%c", a[i], " \n"[i==n]);
    for (int i = 1; i <= n; i++)
        a[i] = _a[a[i]];
    printf("%u\n", calcHash(a, n));
}

int main() {
    for (int i = base[0] = 1; i <= 200000; i++)
        base[i] = 1ull * base[i-1] * B % MOD;
    int tid, T;
    fin(tid), fin(T);
    while (T--) solveSingle();
    return 0;

    int n;
    scanf("%d", &n);
    std::iota(a+1, a+1+n, 1);
    for (int i = 1; i <= n; i++) {
        std::reverse(a+1, a+1+i);
        for (int j = 1; j <= i; j++)
            a[j] = -a[j];
        for (int i = 1; i <= n; i++)
            printf("%d%c", a[i], i==n?'\n':' ');
    }

    return 0;
}