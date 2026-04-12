#include <bits/stdc++.h>
constexpr int N = 200005;
constexpr unsigned B1 = 154, B2 = (int)1e9+7, MOD = (int)1e9 + 9;
int a[N], ans[N << 1];
unsigned pw1[N];
uint64_t pw2[N];
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
uint64_t fHashPre[N], fHashSuf[N], fHashSuf2[N], gHashPre[N];
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
            for (int p = i-len+1, j = n-cl, k = cr-n, l = cl, r = cr+1; p <= i; p++, j++, k++, l--, r++) {
                fHashSuf[j+1] = fHashSuf[j] + a[p] * pw2[j];
                fHashPre[k+1] = fHashPre[k] * B2 + a[p];
                ans[l] = ans[r] = a[p];
                // printf("pos %d and %d --> %d\n", l, r, a[p]);
            }
            int l = 0, r = cr - cl + len;
            const int L = cl - len, R = cl;
            for (int mid; l <= r; ) {
                mid = l + r >> 1;
#define calc(hash, base, Pos) ((Pos + mid > n) ?                                    \
        (hash = fHashSuf[n - Pos] * pw2[Pos + mid - n] + fHashPre[Pos + mid - n],   \
         base = 1) :                                                                \
        (hash = fHashSuf[n - Pos] - fHashSuf[n - (Pos + mid)],                      \
         base = pw2[n - (Pos + mid)]))
                uint64_t leftHash, rightHash, leftBase, rightBase;
                calc(leftHash, leftBase, L), calc(rightHash, rightBase, R);
                leftHash *= rightBase, rightHash *= leftBase;
                if (leftHash == rightHash) l = mid + 1;
                else r = mid - 1;
            }
            // printf("Now [%d,%d] c[%d,%d]\n", i-len+1, i, cl, cr);
            // printf("Checking pos %d (%d) and pos %d (%d)\n", L+l, a[L+l], R+l, a[R+l]);
            if (l <= cr - cl + len && ans[L+l] < ans[R+l]) cl -= len;
            else cr += len;
            len = 0;
        }
    }
    for (int i = n - len + 1; i <= n; i++) ans[++cr] = a[i];
    // for (int i = cl+1; i <= cr; i++)
    //     printf("%d%c", ans[i], " \n"[i==cr]);
    printf("%u\n", calcHash(ans + cl, n));
}

int main() {
    for (int i = pw1[0] = pw2[0] = 1; i <= 200000; i++) {
        pw1[i] = 1ull * pw1[i-1] * B1 % MOD;
        pw2[i] = pw2[i-1] * B2;
    }
    int tid, T;
    fin(tid), fin(T);
    while (T--) solveSingle();
    return 0;

    /* int n;
    scanf("%d", &n);
    std::iota(a+1, a+1+n, 1);
    for (int i = 1; i <= n; i++) {
        std::reverse(a+1, a+1+i);
        for (int j = 1; j <= i; j++)
            a[j] = -a[j];
        for (int i = 1; i <= n; i++)
            printf("%d%c", a[i], i==n?'\n':' ');
    } */

    return 0;
}