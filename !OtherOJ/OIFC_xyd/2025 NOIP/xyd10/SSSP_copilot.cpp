#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace FastI {
    char buf[1 << 22], *p1=nullptr, *p2=nullptr;
    inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    template<typename T>
    inline void in(T &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = ch ^ '0', ch = gc(); ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
static char obuf[1 << 22];
struct FastO {
    char *p3;
    const char* p4;
    inline FastO() : p3(obuf), p4(obuf + (sizeof obuf)) {}
    inline ~FastO() { fwrite(obuf, p3-obuf, 1, stdout); }
    inline void put(char ch) {
        if (p3 == p4) fwrite(obuf, sizeof obuf, 1, stdout), p3 = obuf;
        *p3++ = ch;
    }
    inline void operator()(ll x) {
        static int sta[32];
        sta[sta[0] = 1] = (int)(x % 10);
        while (x /= 10) sta[++sta[0]] = (int)(x % 10);
        while (sta[0]) put(char(sta[sta[0]--] | '0'));
    }
    inline void operator()(char ch) { put(ch); }
} out;
const ll INF = 0x3f3f3f3f3f3f3f3fll;
constexpr int N = 500005;
int U[N], V[N];
ll W[N], dist[N], ans[N];
int main() {
    freopen("SSSP.in", "r", stdin);
    freopen("SSSP.out", "w", stdout);
    int T;
    int id;
    in(T), in(id);
    while (T--) {
        int n, m;
        in(n), in(m);
        for (int i = 1; i <= m; i++) {
            int u, v;
            ll w;
            in(u), in(v), in(w);
            U[i] = u;
            V[i] = v;
            W[i] = w;
        }
        vector<unordered_set<int>> ban(m + 1);
        for (int i = 1; i <= m; i++) {
            int k;
            in(k);
            if (k == 0)
                continue;
            ban[i].reserve(k << 1 | 1);
            for (int j = 0; j < k; j++) {
                int t;
                in(t);
                ban[i].insert(t);
            }
        }

        vector<unordered_set<int>> g(n + 1);
        for (int i = 1; i <= m; i++) {
            g[U[i]].insert(i);
        }

        memset(dist, 0x3f, sizeof dist);

        using pli = pair<ll, int>;
        priority_queue<pli, vector<pli>, greater<pli>> pq;
        dist[0] = 0;
        pq.emplace(0, 0);
        V[0] = 1;
        while (!pq.empty()) {
            auto [d, i] = pq.top();
            pq.pop();
            if (d != dist[i])
                continue;
            int x = V[i];
            auto it = g[x].begin();
            while (it != g[x].end()) {
                int j = *it;
                if (ban[j].find(i) != ban[j].end()) {
                    ++it;
                    continue;
                }
                ll nd = dist[i] + W[j];
                if (dist[j] > nd) {
                    dist[j] = nd;
                    pq.emplace(nd, j);
                }
                it = g[x].erase(it);
            }
        }

        memset(ans, 0x3f, sizeof ans);
        ans[1] = 0;
        for (int i = 1; i <= m; i++) {
            int vv = V[i];
            ans[vv] = min(ans[vv], dist[i]);
        }
        out('0');
        for (int v = 2; v <= n; v++) {
            if (ans[v] == INF) out(' '), out('-'), out('1');
            else out(' '), out(ans[v]);
        }
        out('\n');
    }
    return 0;
}
