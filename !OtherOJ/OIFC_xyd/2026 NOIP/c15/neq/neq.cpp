#include <bits/stdc++.h>
constexpr int N = int(1.2e6) + 5;
namespace In {
    char buf[1 << 20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<typename T> inline void in(T &x) {
        char ch = gc();
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = x * 10 + (ch ^ '0');
    }
} using In::in;
int wl[N], wr[N];
constexpr int K = 20;
constexpr uint64_t INF = (uint64_t)-1;
uint64_t a[N], lcmst[N][K+1], mxst[N][K+1];
inline uint64_t lcm(uint64_t x, uint64_t y) {
    if (x == INF || y == INF) return INF;
    __uint128_t res = (__uint128_t)x / std::__gcd(x, y) * y;
    if (res > (1ll<<60)) return INF;
    return res;
}
inline uint64_t rglcm(int l, int r) {
    int k = 31 - __builtin_clz(r - l + 1);
    return lcm(lcmst[l][k], lcmst[r-(1<<k)+1][k]);
}
inline uint64_t rgmax(int l, int r) {
    int k = 31 - __builtin_clz(r - l + 1);
    return std::max(mxst[l][k], mxst[r-(1<<k)+1][k]);
}
int main() {
    freopen("neq.in", "r", stdin);
    freopen("neq.out", "w", stdout);
    int T, n;
    in(T);
    while (T--) {
        in(n);
        for (int i = 1; i <= n; i++) in(a[i]), lcmst[i][0] = a[i], mxst[i][0] = a[i];
        for (int k = 1; k <= K; k++)
            for (int i = 1; i+(1<<k)-1 <= n; i++) {
                lcmst[i][k] = lcm(lcmst[i][k-1], lcmst[i+(1<<k-1)][k-1]);
                mxst[i][k] = std::max(mxst[i][k-1], mxst[i+(1<<k-1)][k-1]);
            }
        std::map<uint64_t, std::vector<int>> mp;
        mp[0].push_back(0);
        uint64_t now = 0, ans = 0;
        for (int i = 1; i <= n; i++) {
            now ^= a[i];
            for (int j = i; j; ) {
                uint64_t jilcm = rglcm(j, i), target;
                if (jilcm == a[j]) {
                    const auto &vec = mp[now ^ a[j]];
                    int l = 0, r = (int)vec.size() - 1;
                    while (l <= r) {
                        int mid = l + r >> 1;
                        if (rglcm(vec[mid] + 1, i) > a[j]) l = mid + 1;
                        else r = mid - 1;
                    }
                    ans += (int)vec.size() - l;
                    target = jilcm * 2;
                } else target = jilcm;
                if (target == INF) break;
                int l = 1, r = j-1;
                while (l <= r) {
                    int mid = l + r >> 1;
                    if (rgmax(mid, i) >= target) l = mid + 1;
                    else r = mid - 1;
                }
                j = r;
            }
            ans += mp[now].size();
            mp[now].push_back(i);
        }
        printf("%llu\n", ans);
        /* for (int i = 1; i <= n; i++) {
            now = 0;
            for (int j = i; j <= n; j++) {
                now ^= a[j];
                if (now % rglcm(i, j) == 0) --ans;
            }
        }
        if (ans != 0) {
            for (int i = 1; i <= n; i++)
                printf("%llu%c", a[i], " \n"[i==n]);
            printf("WA on tc %d, delta %llu\n", 100 - T, -ans);
            return 1;
        } */
    }
}