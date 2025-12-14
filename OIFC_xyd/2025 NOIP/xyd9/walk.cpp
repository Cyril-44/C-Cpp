#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#define umax(x, y) ((x) = std::max((x), (y)))
namespace FastI {
char buf[1 << 23], *p1 = nullptr, *p2 = nullptr;
inline char get() {
    if (p1 == p2)
        p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
    return *p1++;
}
inline void in(int& x) {
    char ch = get();
    while (ch < '0' || ch > '9')
        ch = get();
    for (x = ch ^ '0', ch = get(); ch >= '0' && ch <= '9'; ch = get())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
}  // namespace FastI
int count;
using FastI::in;
struct FastO {
    char buf[1 << 23], *p3 = buf;
    const char* p4;
    inline FastO() : p3(buf), p4(buf + sizeof buf) {}
    inline ~FastO() { fwrite(buf, 1, p3 - buf, stdout); }
    inline void put(char x) {
        if (p3 == p4) {
            fwrite(buf, 64, 1 << 17, stdout);
            p3 = buf;
        }
        *p3++ = x;
    }
    inline void operator()(char x) { put(x); }
    inline void operator()(int x) {
        ++count;
        static int sta[20];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10)
            sta[++sta[0]] = x % 10;
        while (sta[0])
            put(sta[sta[0]--] | '0');
    }
    inline void operator()(const char* s) {
        char* p = const_cast<char*>(s);
        while (*p)
            put(*p++);
    }
} out;
constexpr int N = 100005;
struct Edge {
    int to, nxt;
} e[N << 1];
int head[N];
inline void addedg(int fr, int to) {
    e[++head[0]] = Edge{ to, head[fr] };
    head[fr] = head[0];
}
int furpnt, fa[N], dep[N], sz[N];
// 求最远点
void dfs1(int u, int pre) {
    fa[u] = pre, sz[u] = 1, dep[u] = dep[pre] + 1;
    if (dep[u] > dep[furpnt])
        furpnt = u;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if ((v = e[i].to) == pre)
            continue;
        dfs1(v, u);
        sz[u] += sz[v];
    }
}
int cnt;
bool vis[N];
// dfs 输出
void dfs2(int u) {
    if (!cnt)
        return;
    int tmp = --cnt;
    vis[u] = true;
    out(u), out(' ');
    for (int i = head[u], v; i; i = e[i].nxt)
        if (!vis[v = e[i].to])
            dfs2(v);
    if (tmp != cnt)
        out(u), out(' ');
}
int main() {
    freopen("walk.in", "r", stdin);
    freopen("walk.out", "w", stdout);
    int T, n, m, k;
    in(T);
    while (T--) {
        in(n), in(m), in(k);
        memset(head, 0, sizeof(int) * (n + 1));
        for (int i = 1, u, v; i < n; i++) {
            in(u), in(v);
            addedg(u, v);
            addedg(v, u);
        }

        // 求直径
        furpnt = 0, dfs1(1, 0);
        int u = furpnt;
        furpnt = 0, dfs1(u, 0);
        int v = furpnt;
        std::vector<int> chain;
        chain.reserve(dep[v]);
        for (int i = v; i != u; i = fa[i])
            chain.push_back(i);
        chain.push_back(u);
        std::reverse(chain.begin(), chain.end());

        int clen = dep[v];  // 直径长度
        if (k > m || (k > clen && (k - clen) * 2 > m - clen)) {
            out("No\n");
            continue;
        }
        out("Yes\n");
        if (k <= clen) {
            for (int i = 0; i < k; i++)
                out(chain[i]), out(' ');
            int u = chain[k - 1], v = chain[k - 2];
            for (int tp = m - k; tp; --tp)
                out(v), out(' '), std::swap(u, v);
        } else {
            memset(vis, 0, sizeof(bool) * (n + 1));
            for (int o : chain)
                vis[o] = true;
            cnt = k - clen;
            for (int u : chain) {
                dfs2(u);
            }
            fprintf(stderr, "%d\n", count);
            int u = chain[clen-1], v = chain[clen-2];
            for (int tp = m - clen - (k - clen) * 2; tp > 0; --tp)
                out(v), out(' '), std::swap(u, v);
        }
        out('\n');
        count = 0;
    }
    return 0;
}
