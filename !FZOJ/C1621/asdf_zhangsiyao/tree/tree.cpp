#include <bits/stdc++.h>
constexpr int N = 200005;
#ifdef CLANGD
#define N 25
#endif
constexpr uint64_t B = 31, MD = (uint64_t)1e16 + 99;
std::vector<std::pair<int, char>> g[N];
char s[N];
int64_t ans[N];

struct Edge { int to, nxt; } e[N];
int head[N], edgtot;
inline void addedg(const int &fr, const int &to) {
    e[++edgtot] = {to, head[fr]};
    head[fr] = edgtot;
}
struct Node {
    int ch[26];
    int fail;
    std::vector<int> occurrence;
    Node& operator()(int x);
    inline int& operator[](int x) { return ~x ? ch[x] : fail; }
} ac[N];
inline Node& Node::operator()(int x) { return ac[(*this)[x]]; }
namespace ACM {
int tot = 0;
#define FAIL -1
inline void insert(char *s, int id) {
    int u = 0;
    for (; *s; ++s) {
        if (!ac[u][*s - 'a']) ac[u][*s - 'a'] = ++tot;
        u = ac[u][*s - 'a'];
    }
    ac[u].occurrence.push_back(id);
}
static int q[N << 2];
inline void buildAC() {
    int l = 0, r = 0;
    for (int i = 0; i < 26; ++i)
        if (ac[0][i]) {
            q[r++] = ac[0][i];
            addedg(0, ac[0][i]);
        }
    while (l ^ r) {
        int u = q[l++];
        for (int i = 0; i < 26; ++i)
            if (ac[u][i]) {
                ac[u](i)[FAIL] = ac[u](FAIL)[i];
                addedg(ac[u](FAIL)[i], ac[u][i]);
                q[r++] = ac[u][i];
            }
            else ac[u][i] = ac[u](FAIL)[i];
    }
}
long long f[N];
inline void dfs(int u) {
    for (int i = head[u]; i; i = e[i].nxt) {
        dfs(e[i].to);
        f[u] += f[e[i].to];
    }
    for (int j : ac[u].occurrence)
        ans[j] += f[u];
}
inline void solve(char *s) {
    int u = 0;
    memset(f, 0, sizeof f);
    for (; *s; ++s) {
        u = ac[u][*s - 'a'];
        ++f[u];
    }
    dfs(0);
}
#undef FAIL
}

int main() {
    freopen("tree.in", "r", stdin);
    freopen("tree.out", "w", stdout);
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i < n; i++) {
        int u, v; char ch; scanf("%d%d %c", &u, &v, &ch);
        g[u].emplace_back(v, ch), g[v].emplace_back(u, ch);
    }
    if (n <= 5000) {
        static uint64_t val[5005];
        static int que[5005], fa[5005];
        std::map<uint64_t, int> mp;
        for (int u = 1, tl; u <= n; u++) {
            que[tl=1] = u, val[u] = 0, fa[u] = 0;
            for (int i = 1; i <= n; i++) {
                int x = que[i];
                for (auto [y, c] : g[x]) if (y != fa[x]) {
                    val[y] = (val[x] * B + (c - 'a' + 1));
                    fa[y] = x; ++mp[val[y]];
                    que[++tl] = y;
                }
            }
        }
        for (int i = 1; i <= q; i++) {
            scanf("%s", s);
            uint64_t hash = 0;
            for (int j = 0; s[j]; j++)
                hash = (hash * B + (s[j] - 'a' + 1));
            auto it = mp.find(hash);
            printf("%d\n", it != mp.end() ? it->second : 0);
        }
    } else {
        for (int i = 1; i <= q; i++) {
            scanf("%s", s);
            ACM::insert(s, i);
        }
        ACM::buildAC();
        int rt = -1, pre = -1;
        for (int i = 1; i <= n; i++)
            if (g[i].size() == 1) { rt = i; break; }
        int u = rt;
        for (int i = 1; i < n; i++) {
            for (auto [v, c] : g[u]) if (v != pre) {
                s[i] = c, pre = u, u = v; break;
            }
        }
        s[n] = '\0';
        ACM::solve(s+1);
        std::reverse(s+1, s+n);
        ACM::solve(s+1);
        for (int i = 1; i <= q; i++)
            printf("%lld\n", ans[i]);
    }
}