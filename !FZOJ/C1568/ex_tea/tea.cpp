#include <bits/stdc++.h>
constexpr int N = 200005, B1 = 154, B2 = 233, MOD = (int)1e9 + 9;
int a[N], ans[N];
unsigned pw1[N], pw2[N];
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
        if ((ans += 1ull * a[i] * pw1[i-1] % MOD) >= MOD)
            ans -= MOD;
    return ans;
}
int n;
uint64_t fHashPre[N], fHashSuf[N], gHashPre[N];
// fpre 记录往后拼接的，fsuf 记录往前拼接的
/*
相当于每个开启的点隔开，切成若干段
从左到右，将每一段添加到构造串的左边或者右边
每次贪心去比较即可，选字典序小的。
注意到比较的时候可以先拎出来 LCP 然后比较下一位。
*/
inline void solveSingle() {
    int m;
    fin(n), fin(m);
    on.set();
    for (int i = 1; i <= n; i++) fin(a[i]);
    for (int i = 1, bi; i <= m; i++) {
        fin(bi);
        on.reset(bi);
    }
    int cl = n, cr = n, len = 0; // Constructed (l, r]
    for (int i = 1; i <= n; i++) {
        ++len;
        if (on[i]) {
            for (int j = i - len + 1, l = n - cl, r = cr - n; j <= i; j++, l++, r++) {
                fHashSuf[l+1] = fHashPre[l] * B2 + a[j];
                fHashPre[r+1] = fHashPre[r] + a[j] * pw2[r];
            }
            int l = 0, r = cr - cl + len;
            const int L = cl - len, R = cl;
            for (int mid; l <= r; ) {
                mid = l + r >> 1;
#define calc(Pos) (Pos + mid > n) ?                                    \
    fHashSuf[n - Pos] * pw2[Pos + mid - n] + fHashPre[Pos + mid - n] : \
    fHashSuf[n - Pos] - fHashSuf[n - (Pos + mid)] * pw2[mid]
                uint64_t leftHash = calc(L), rightHash = calc(R);
                if (leftHash == rightHash) r = mid - 1;
                else l = mid + 1;
            }
            if (l == cr - cl + len || 1);
        }
    }
}

int main() {
    for (int i = pw1[0] = pw2[0] = 1; i <= 200000; i++) {
        pw1[i] = 1ull * pw1[i-1] * B1 % MOD;
        pw2[i] = 1ull * pw2[i-1] * B2 % MOD;
    }
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