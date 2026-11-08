
#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 25, M = 405;
#else
constexpr int N = 200005, M = 400005;
#endif
using U = uint64_t;
const U B = (std::chrono::steady_clock::now().time_since_epoch().count() ^ 0x9e3779b97f4a7c15ULL) | 1ULL;
U base[M], pref[M];
std::vector<std::pair<int, char>> g[N];
char s[M];
int end[N], m, q, sqrtm, rep[N];
bool haslen[N];
int64_t ans[N];
std::unordered_map<U, int> mp;
std::vector<int> ids;
U geth(int l, int r) { return pref[r] - pref[l] * base[r-l]; }
U xs64(U x) {
    x ^= x << 11;
    x ^= x >> 45;
    x ^= x << 14;
    return x;
}
U key(U h, int l) { return h ^ xs64(l); }
namespace DVD {
    bool vis[N];
    int sz[N], fa[N], centroid, overallSz;
    struct Hsh { U f, r; int d; };
    struct Br { int v, t; char c; };
    struct State { int u, p, d; U f, r; };
    std::vector<Hsh> hash[N];
    std::vector<State> st;
    int rt;
    void dfs1(int u, int p) {
        std::vector<int> ord;
        ord.reserve(overallSz);
        fa[u] = p, ord.push_back(u);
        for (int i = 0; i < (int)ord.size(); i++) {
            int x = ord[i];
            for (auto [v, c] : g[x]) if (v != fa[x] && !vis[v])
                fa[v] = x, ord.push_back(v);
        }
        int best = overallSz;
        for (int i = (int)ord.size()-1; i >= 0; i--) {
            int x = ord[i], mx = 0;
            sz[x] = 1;
            for (auto [v, c] : g[x]) if (v != fa[x] && !vis[v]) {
                sz[x] += sz[v];
                mx = std::max(mx, sz[v]);
            }
            mx = std::max(mx, overallSz - sz[x]);
            if (mx < best) best = mx, centroid = x;
        }
    }
    void dfs2(int u, int p, U now) {
        st.clear();
        st.push_back({u, p, 1, now, now});
        while (!st.empty()) {
            auto [x, f, d, h1, h2] = st.back(); st.pop_back();
            hash[rt].push_back({h1, h2, d});
            for (auto [v, c] : g[x]) if (v != f && !vis[v]) {
                U t = c - 'a' + 1;
                st.push_back({v, x, d+1, h1 * B + t, h2 + t * base[d]});
            }
        }
    }
    void work(const std::unordered_map<U, int> &a, const std::unordered_map<U, int> &b, int op, int lim, bool sub) {
        for (int i : ids) {
            int l = end[i-1], r = end[i], len = r-l;
            if (len > lim) break;
            for (int j = sub; j <= len - sub; j++) {
                auto it = a.find(key(geth(l, l+j), j));
                if (it == a.end()) continue;
                auto jt = b.find(key(geth(l+j, r), len-j));
                if (jt != b.end()) ans[i] += (int64_t)op * it->second * jt->second;
            }
        }
    }
    void calc(int u, int p, U h, int d) {
        if (d && haslen[d]) {
            auto it = mp.find(key(h, d));
            if (it != mp.end()) ans[it->second]++;
        }
        for (auto [v, c] : g[u]) if (v != p && !vis[v])
            calc(v, u, h * B + (c - 'a' + 1), d+1);
    }
    void brute(int u) {
        std::vector<int> ord{u};
        fa[u] = 0;
        for (int i = 0; i < (int)ord.size(); i++) {
            int x = ord[i];
            for (auto [v, c] : g[x]) if (v != fa[x] && !vis[v])
                fa[v] = x, ord.push_back(v);
        }
        for (int x : ord) calc(x, 0, 0, 0);
    }
    void dfs(int u, int s) {
        if (s <= sqrtm) return brute(u);
        overallSz = s, dfs1(u, 0);
        u = centroid, vis[u] = true;
        std::vector<Br> ch;
        for (auto [v, c] : g[u]) if (!vis[v])
            ch.push_back({v, v == fa[u] ? s - sz[u] : sz[v], c});
        int dep = 0;
        for (auto [v, t, c] : ch) {
            rt = v;
            dfs2(v, u, c - 'a' + 1);
            for (auto &x : hash[v]) dep = std::max(dep, x.d);
        }
        std::unordered_map<U, int> a, b;
        a.reserve(s * 2), b.reserve(s * 2);
        a[0] = b[0] = 1;
        for (auto [v, t, c] : ch) for (auto &x : hash[v])
            ++a[key(x.r, x.d)], ++b[key(x.f, x.d)];
        work(a, b, 1, std::min(s-1, dep*2), false);
        for (auto [v, t, c] : ch) {
            if (t > sqrtm) {
                a.clear(), b.clear();
                dep = 0;
                for (auto &x : hash[v]) {
                    ++a[key(x.r, x.d)], ++b[key(x.f, x.d)];
                    dep = std::max(dep, x.d);
                }
                work(a, b, -1, std::min(s-1, dep*2), true);
            } else {
                for (auto &x : hash[v]) for (auto &y : hash[v]) {
                    int len = x.d + y.d;
                    if (len >= s || !haslen[len]) continue;
                    auto it = mp.find(key(x.r * base[y.d] + y.f, len));
                    if (it != mp.end()) ans[it->second]--;
                }
            }
        }
        for (auto [v, t, c] : ch) {
            std::vector<Hsh>().swap(hash[v]);
            dfs(v, t);
        }
    }
}
int main() {
    int n;
    scanf("%d%d", &n, &q);
    for (int i = 1; i < n; i++) {
        int u, v; char ch;
        scanf("%d%d %c", &u, &v, &ch);
        g[u].emplace_back(v, ch), g[v].emplace_back(u, ch);
    }
    for (int i = 1; i <= q; i++) {
        scanf("%s", s + end[i-1]);
        end[i] = end[i-1] + strlen(s + end[i-1]);
    }
    m = end[q], sqrtm = std::max(1, (int)sqrt(m));
    base[0] = 1;
    for (int i = 1; i < M; i++) base[i] = base[i-1] * B;
    for (int i = 1; i <= m; i++) pref[i] = pref[i-1] * B + s[i-1] - 'a' + 1;
    mp.reserve(q * 2);
    DVD::st.reserve(n);
    for (int i = 1; i <= q; i++) {
        int len = end[i] - end[i-1];
        if (len >= n) continue;
        auto [it, ok] = mp.emplace(key(geth(end[i-1], end[i]), len), i);
        rep[i] = it->second;
        if (ok) ids.push_back(i), haslen[len] = true;
    }
    std::sort(ids.begin(), ids.end(), [](int x, int y) {
        return end[x] - end[x-1] < end[y] - end[y-1];
    });
    DVD::dfs(1, n);
    for (int i = 1; i <= q; i++) printf("%lld\n", (long long)ans[rep[i]]);
}
