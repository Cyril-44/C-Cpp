#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <vector>
#define umin(x, y) (((x) < (y)) ? (x) : (x) = (y), void())
constexpr int N = 100005, M = 1000005, MX = 1e5, K = 17, INF = 0x7fffffff;
using ULL = unsigned long long;
constexpr ULL MOD = 998244353;
int a[N], n, k;
unsigned b[N], c[N];
namespace SegTree {
struct Node {
    int ls, rs, mn, tag;
    inline void pull(int tg) {
        if (tg ^ INF) {
            umin(mn, tg);
            umin(tag, tg);
        }
    }
} tr[N * (K+1) << 2]{{0, 0, INF, INF}};
int tot, rt[N], map[N];
#define Ls(u) tr[u].ls
#define tls(u) tr[Ls(u)]
#define Rs(u) tr[u].rs
#define trs(u) tr[Rs(u)]

inline void pushup(int u) {
    tr[u].mn = std::min(tls(u).mn, trs(u).mn);
}
int X, L, R;
void upd(int &u, int l, int r, int tg) {
    tr[++tot] = tr[u];
    u = tot;
    tr[u].pull(tg);
    if (L <= l && r <= R) {
        return umin(tr[u].mn, X);
    }
    int mid = l + r >> 1;
    if (L <= mid) upd(Ls(u), l, mid, tr[u].tag);
    if (mid < R) upd(Rs(u), mid+1, r, tr[u].tag);
    pushup(u); tr[u].tag = INF;
}
int que(int u, int l, int r, int tg) {
    if (!u) return INF;
    tr[u].pull(tg);
    if (L <= l && r <= R) return tr[u].mn;
    int mid = l + r >> 1;
    int res = INF;
    if (L <= mid) umin(res, que(Ls(u), l, mid, tr[u].tag));
    if (mid < R) umin(res, que(Rs(u), mid+1, r, tr[u].tag));
    tr[u].tag = INF;
    return res;
}
inline void update(int p, int x) { // val[p].back() = x ==> chkmn [x, n] <- p
    ++map[0];
    rt[map[0]] = rt[map[0] - 1];
    map[p] = map[0];
    L = x, R = MX, X = p;
    upd(rt[map[0]], 0, MX, INF);
}
inline int query(int p, int x) { // x >= val[i \in p+1 .. n].back()   Query min i
    L = x, R = MX;
    return que(rt[map[p]-1], 0, MX, INF);
}

#undef Ls
#undef tls
#undef Rs
#undef trs
}
std::vector<int> pos[N];
inline void finish() { fclose(stdout); exit(0); }
void dfs(int l, int now, unsigned val) {
    if (now == N) return;
    int cnt = pos[now].end() - std::upper_bound(pos[now].begin(), pos[now].end(), l);
    size_t curr = 1;
    for (int len = 1; len <= cnt; ++len) {
        curr = curr * (cnt - len + 1) / len;
        if (curr > k) curr = k;
        auto val_now = ((ULL)val * b[len] + (ULL)now * c[len-1]) % MOD;
        for (auto tp = curr; tp; --tp) printf("%llu\n", val_now);
        k -= curr;
        if (k == 0) finish();
    }
    int tnow = SegTree::query(now, l);
    // int tnow1 = now + 1;
    // while (tnow1 < N && (pos[tnow1].empty() || pos[tnow1].back() < l)) ++tnow1;
    // fprintf(stderr, "(%d,%d) %d %d\n", now, l, tnow, tnow1);
    if (tnow == INF) return;
    for (int len = cnt; len >= 1; --len) {
        dfs(*(pos[now].rbegin() + cnt - len), tnow, ((ULL)val * b[len] + (ULL)now * c[len-1]) % MOD);
    }
    dfs(l, tnow, val);
}
int main() {
    scanf("%d%d%d", &n, &k, &b[1]);
    b[0] = 1;
    for (int i = 2; i <= n; i++) b[i] = (ULL)b[i-1] * b[1] % MOD;
    c[0] = 1;
    for (int i = 1; i <= n; i++) c[i] = (c[i-1] + b[i]) % MOD;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        pos[a[i]].push_back(i);
    }
    for (int i = MX; i >= 1; i--)
        if (!pos[i].empty())
            SegTree::update(i, pos[i].back());
    for (int i = 1; i <= n; i++)
        if (!pos[i].empty()) dfs(0, i, 0);
    puts("What the fuck???");
    return 0;
}