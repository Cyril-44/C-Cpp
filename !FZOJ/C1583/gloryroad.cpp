#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
constexpr int N = 100005;
std::vector<std::vector<int>> g;
struct FastI {
    char get() {
        if (p1 == p2) p2 = (*buf='\0', p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    void operator()(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
private:
    char buf[1 << 20], *p1{}, *p2{};
} fin; 
int n;
struct FastO {
    FastO() : p3(buf), p4(buf + sizeof(buf)) {}
    ~FastO() { flush(); }
    void flush() { fwrite(buf, p3-buf, 1, stdout); }
    void put(char ch) {
        if (p3 == p4) fwrite(buf, sizeof buf, 1, stdout), p3 = buf;
        *p3++ = ch;
    }
    void operator()(int x) {
        if (x == -1) return put('-'), put('1');
        static unsigned char buf[20];
        buf[buf[0] = 1] = x % 10;
        while (x /= 10) buf[++buf[0]] = x % 10;
        while (buf[0]) put(buf[buf[0]--] | '0');
    }
    void operator()(const std::vector<int> &vec) {
        For(i, 1, n) (*this)(vec[i]), put(' ');
        put('\n');
    }
private:
    char buf[1 << 20], *p3;
    const char *p4;
} fout;
std::vector<int> bfsGetDis(int st, const std::vector<std::vector<int>> &ng) {
    std::vector<int> dis(ng.size(), -1);
    std::queue<int> q;
    q.push(st); dis[st] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : ng[u]) if (dis[v] == -1)
            dis[v] = dis[u] + 1, q.push(v);
    }
    return dis;
}
class BF {
    std::vector<std::vector<int>> ng;
    int st, rt, len;
    void dfs(int u, int fa = 0, int dep = 0) {
        if (dep == len) ng[rt].push_back(u);
        else for (int v : g[u]) if (v != fa)
            dfs(v, u, dep+1);
    }
public:
    BF(int u_, int a) : ng(n+1), st(u_), len(a) {}
    void work() {
        For(i, 1, n) dfs(rt = i);
        fout(bfsGetDis(st, ng));
    }
};
class BF2 {
    std::vector<std::vector<int>> ng;
    int st;
    void dfs(int u, int fa = 0) {
        for (int v : g[u]) if (v != fa)
            for (int vv : g[v]) if (vv != u) {
                ng[u].push_back(vv);
                dfs(vv, v);
            }
    }
public:
    BF2(int u_) : ng(n+1), st(u_) {}
    void work() {
        dfs(st);
        fout(bfsGetDis(st, ng));
    }
};
class BFC {
    std::vector<int> chain, chainpos;
    std::vector<std::vector<int>> ng;
    int st, len;
public:
    BFC(int u, int a, const std::vector<int> &deg) : chainpos(n+1), ng(n+1), st(u), len(a) {
        int rt = -1;
        For(i, 1, n) if (deg[i] == 1) {rt = i; break;}
        int fa = 0; u = rt;
        chain.reserve(n);
        chain.push_back(u);
        do {
            for (int v : g[u]) if (v != fa) {
                fa = u; u = v;
                chain.push_back(v);
                break;
            }
        } while (deg[u] != 1);
        For(i, 0, n-1) chainpos[chain[i]] = i;
    }
    void work() {
        For(i, 1, n) {
            int pos = chainpos[i];
            if (pos - len >= 0) ng[i].push_back(chain[pos-len]);
            if (pos + len < n) ng[i].push_back(chain[pos+len]);
        }
        fout(bfsGetDis(st, ng));
    }
};
int main() {
    int T; fin(T);
    while (T--) {
        int q; fin(n), fin(q);
        g.assign(n+1, {});
        std::vector<int> deg(n+1);
        bool isChain = true;
        For(i, 1, n-1) {
            int u, v; fin(u), fin(v);
            g[u].push_back(v), g[v].push_back(u);
            ++deg[u], ++deg[v];
            if (isChain && (deg[u] > 2 || deg[v] > 2)) isChain = false;
        }
        For(i, 1, q) {
            int u, a; fin(u), fin(a);
            if (a == n) {
                For(i, 1, n) fout(i == u ? 0 : -1), fout.put(' ');
                fout.put('\n');
            }
            else if (a == 1) fout(bfsGetDis(u, g));
            else if (a == 2) BF2(u).work();
            else if (isChain) BFC(u, a, deg).work();
            else if (n <= 1000 || true) BF(u, a).work();
        }
    }
    return 0;
}