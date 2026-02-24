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
int a[N], b[N], pre[N], nxt[N], eq[N], l[N], r[N], p[N];
inline void print(int n, int *a) {
    for (int i = 1; i <= n; i++) if (~a[i]) printf("%2d ", a[i]); else printf("__ ");
    putchar('\n');
}
inline void in(int n, int x, int *a) {
    nxt[n+1] = 0;
    a[n+1] = 0;
    for (int i = 1; i <= n; i++) in(a[i]);
    a[x] = n;
    for (int i = 1; i <= n; i++) pre[i] = ~a[i] ? i : pre[i-1];
    for (int i = n; i >= 1; i--) nxt[i] = ~a[i] ? i : nxt[i+1];
    for (int i = 1; i <= n; i++) if (a[i] == -1 && a[pre[i]] == a[nxt[i]]) a[i] = a[pre[i]];
}
inline void umin(int &x, int y) { x > y && (x = y); }
inline void umax(int &x, int y) { x < y && (x = y); }
int sz[N], len[N];
int main() {
    freopen("legend.in", "r", stdin);
    freopen("legend.out", "w", stdout);
    int T, n; in(T);
    while (T--) {
        in(n); in(n, n, a), in(n, 1, b);
        memset(p, -1, sizeof p);
        for (int i = 0; i < n; i++) l[i] = 1, r[i] = n;
        for (int i = 1; i <= n; i++) {
            pre[i] = ~a[i-1] ? i-1 : pre[i-1];
            eq[i] = a[i] == a[i-1] ? eq[i-1] : i;
            // if (~a[i-1] && ~a[i] && a[i] != a[i-1]) p[i] = a[i-1];
            if (a[pre[i]] < a[i])
                if (pre[i] == i-1) p[i] = a[i-1];
                else {
                    int x = a[pre[i]];
                    umax(l[x], pre[i] + 1), umin(r[x], i);
                    for (x++; x < a[i]; x++) umin(r[x], i);
                    // printf("%d <-- [%d,%d]\n", a[pre[i]], pre[i] + 1, i);
                    // printf("[%d,%d] <-- [%d,+oo)\n", a[pre[i]]+1, a[i] - 1, i);
                }
        }
        nxt[n+1] = n+1, eq[n+1] = n+1;
        for (int i = n; i >= 1; i--) {
            nxt[i] = ~b[i+1] ? i+1 : nxt[i+1];
            eq[i] = b[i] == b[i+1] ? eq[i+1] : i;
            if (b[nxt[i]] < b[i])
                if (nxt[i] == i+1) p[i] = b[i+1];
                else {
                    int x = b[nxt[i]];
                    umax(l[x], i), umin(r[x], nxt[i] - 1);
                    for (x++; x < b[i]; x++) umax(l[x], i);
                    // printf("%d <-- [%d,%d]\n", b[nxt[i]], i, nxt[i] - 1);
                    // printf("[%d,%d] <-- (-oo,%d]\n", b[nxt[i]]+1, b[i] - 1, i);
                }
        }
        std::set<int> st;
        for (int i = 1; i <= n; i++)
            if (~p[i]) st.insert(p[i]);
        for (int i = 1, idx = 0; i <= n; i++) // p_1~i 中 一定有 0~ai-1
            for (; idx < a[i]; idx++) if (!st.count(idx)) umin(r[idx], i);
        for (int i = n, idx = 0; i >= 1; i--) // p_i~n 中 一定有 0~bi-1
            for (; idx < b[i]; idx++) if (!st.count(idx)) umax(l[idx], i);
        
        for (int i = 1; i <= n; i++) len[i] = 1, sz[i] = p[i] == -1;
        int ans = 1;
        for (int i = 0; i < n; i++) {
            if (st.count(i)) continue;
            int u = l[i];
            for (int j = u + len[u]; j <= r[i]; j += len[j])
                len[u] += len[j], sz[u] += sz[j];
            ans = 1ull * ans * sz[u]-- % MOD;
        }
        printf("%d\n", ans);
        /* std::vector<std::pair<int,int>> ranges;
        ranges.reserve(n - st.size());
        for (int i = 0; i < n; i++)
            if (!st.count(i))
                ranges.emplace_back(l[i], r[i]);
        std::sort(ranges.begin(), ranges.end(), [](auto&x,auto&y){return x.second-x.first<y.second-y.first;});
        print(n, a), print(n, b), print(n, p);
        for (int i = 0; i < n; i++) if (!st.count(i)) printf("%d: [%d, %d]\n", i, l[i], r[i]);
        if (ranges.empty()) puts("1");
        else {
            int aval = 0, L = ranges.front().first, R = ranges.front().second;
            for (int i = L; i <= R; i++) if (p[i] == -1) ++aval;
            int ans = aval--;
            for (int i = 1; i < (int)ranges.size(); i++) {
                while (L > ranges[i].first) if (p[--L] == -1) ++aval;
                while (R < ranges[i].second) if (p[++R] == -1) ++aval;
                ans = 1ull * ans * aval-- % MOD;
            }
            printf("%d\n", ans);
        } */
    }
    return 0;
}