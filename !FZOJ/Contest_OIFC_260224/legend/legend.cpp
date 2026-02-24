#include <cstdio>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
namespace FastI {
char buf[1 << 20], *p1{}, *p2{};
inline char gc() {
    if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
    return *p1++;
}
inline void in(int &x) {
    char ch = gc();
    while (ch != '-' && (ch < '0' || ch > '9')) ch = gc();
    if (ch == '-') x = -1, gc();
    else for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
        x = (x << 3) + (x << 1) + (ch & 0xf);
}
} using FastI::in;
constexpr int N = (int)1e6 + 5, MOD = 998244353;
int a[N], b[N], pre[N], nxt[N], l[N], r[N], p[N];
inline void print(int n, int *a) {
    for (int i = 1; i <= n; i++) if (~a[i]) printf("%2d ", a[i]); else printf("__ ");
    putchar('\n');
}
inline void in(int n, int *a) {
    nxt[n+1] = 0;
    a[n+1] = 0;
    for (int i = 1; i <= n; i++) in(a[i]);
    for (int i = 1; i <= n; i++) pre[i] = ~a[i] ? i : pre[i-1];
    for (int i = n; i >= 1; i--) nxt[i] = ~a[i] ? i : nxt[i+1];
    for (int i = 1; i <= n; i++) if (a[i] == -1 && a[pre[i]] == a[nxt[i]]) a[i] = a[pre[i]];
}
int main() {
    freopen("legend.in", "r", stdin);
    freopen("legend.out", "w", stdout);
    int T, n; in(T);
    while (T--) {
        in(n);
        in(n, a), in(n, b);
        a[n] = b[1] = n;
        memset(p, -1, sizeof p);
        for (int i = 1; i <= n; i++) {
            if (a[i] == -1) a[i] = a[i-1], pre[i] = pre[i-1];
            else pre[i] = i;
        }
        for (int i = n; i >= 1; i--) {
            if (b[i] == -1) b[i] = b[i+1], nxt[i] = nxt[i+1];
            else nxt[i] = i;
        }
        for (int i = 1; i <= n; i++) if (~a[i] && ~a[i-1] && a[i] != a[i-1]) p[i] = a[i-1];
        for (int i = n; i >= 1; i--) if (~b[i] && ~b[i+1] && b[i] != b[i+1]) p[i] = b[i+1];
        std::set<int> st;
        bool flg = false;
        for (int i = 1; i <= n; i++)
            if (~p[i]) if (!st.insert(p[i]).second) flg = true;
        for (int i = 1, idx = 0; i <= n; i++) // p_1~i 中 一定有 0~ai-1
            for (; idx < a[i]; idx++) if (!st.count(idx)) r[idx] = nxt[i];
        for (int i = n, idx = 0; i >= 1; i--) // p_i~n 中 一定有 0~bi-1
            for (; idx < b[i]; idx++) if (!st.count(idx)) l[idx] = pre[i];
        std::vector<std::pair<int,int>> ranges;
        ranges.reserve(n - st.size());
        for (int i = 0; i < n; i++)
            if (!st.count(i))
                ranges.emplace_back(l[i], r[i]);
        std::sort(ranges.begin(), ranges.end(), [](auto&x,auto&y){return x.second-x.first<y.second-y.first;});

        /* print(n, a), print(n, b), print(n, p);
        for (int i = 0; i < (int)ranges.size(); i++) {
            printf("[%d, %d]\n",  ranges[i].first, ranges[i].second);
            if (!(ranges[i].first <= ranges[i-1].first && ranges[i-1].second <= ranges[i].second))
                throw;
        } */

        int ans = 1;
        if (!ranges.empty()) {
            int aval = flg, l = ranges.front().first, r = ranges.front().second;
            for (int i = l; i <= r; i++) if (p[i] == -1) ++aval;
            for (const auto &pir : ranges) {
                while (l > pir.first) if (p[--l] == -1) ++aval;
                while (r < pir.second) if (p[++r] == -1) ++aval;
                // printf("*%d ", aval);
                ans = 1ull * ans * aval-- % MOD;
            }

        }
        printf("%d\n", ans);
    }
    return 0;
}