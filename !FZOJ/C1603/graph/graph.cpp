#include <bits/stdc++.h>
constexpr int N = 1000004;
namespace fasti{using namespace std;const int bufl=1<<20;const double base1[16]={1,1e-1,1e-2,1e-3,1e-4,1e-5,1e-6,1e-7,1e-8,1e-9,1e-10,1e-11,1e-12,1e-13,1e-14,1e-15};const double base2[16]={1,1e1,1e2,1e3,1e4,1e5,1e6,1e7,1e8,1e9,1e10,1e11,1e12,1e13,1e14,1e15};struct IN{FILE*IT;char ibuf[bufl],*is=ibuf,*it=ibuf;IN(){IT=stdin;}IN(char*a){IT=fopen(a,"r");}inline char getChar(){if(is==it){it=(is=ibuf)+fread(ibuf,1,bufl,IT);if(is==it)return EOF;}return*is++;}template<typename Tp>inline void getInt(Tp&a){a=0;int b=0,c=getChar();while(c<48||c>57)b^=(c==45),c=getChar();while(c>=48&&c<=57)a=(a<<1)+(a<<3)+c-48,c=getChar();if(b)a=-a;}template<typename Tp>inline void getDouble(Tp&a){a=0;int b=0,c=getChar(),d=0;__int128 e=0,f=0;while(c<48||c>57)b^=(c==45),c=getChar();while(c>=48&&c<=57)e=(e<<1)+(e<<3)+c-48,c=getChar();if(c==46){c=getChar();while(c>=48&&c<=57)d++,f=(f<<1)+(f<<3)+c-48,c=getChar();}a=e+base1[d]*f;if(b)a=-a;}IN&operator>>(char&a){a=getChar();return*this;}IN&operator>>(char*a){do{*a=getChar();}while(*a<=32);while(*a>32)*++a=getChar();*a=0;return*this;}IN&operator>>(string&a){a="0";char b=getChar();while(b<=32)b=getChar();while(b>32)a+=b,b=getChar();return*this;}IN&operator>>(int&a){getInt(a);return*this;}IN&operator>>(long long&a){getInt(a);return*this;}IN&operator>>(unsigned long long&a){getInt(a);return*this;}IN&operator>>(__int128&a){getInt(a);return*this;}IN&operator>>(float&a){getDouble(a);return*this;}IN&operator>>(double&a){getDouble(a);return*this;}IN&operator>>(long double&a){getDouble(a);return*this;}};}fasti::IN fin;
struct UFS {
    int fa[N], sz[N];
    void init(int n) { std::iota(fa, fa+1+n, 0), std::fill(sz+1, sz+1+n, 1); }
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    bool unite(int u, int v) {
        if (!v) return true;
        u = find(u), v = find(v);
        if (u == v) return false;
        fa[u] = v; return true;
    }
} ufs;
std::vector<int> g[N];
struct Edge { int u, v; } edgs[N];
struct Ops { int op, x, y; } ops[N];
bool del[N], ans[N];
int fa[N], cnt[N], dep[N], top = 0;
inline void insedg(const Edge& e) {
    if (ufs.unite(e.u, e.v)) {
        g[e.u].push_back(e.v);
        g[e.v].push_back(e.u);
    }
}
void dfs(int u) {
    for (int v : g[u]) if (v != fa[u])
        fa[v] = u, dep[v] = dep[u] + 1, dfs(v);
}
inline void coveredg(const Edge& e) { // 覆盖树上 e.u --- e.v
    int u = ufs.find(e.u), v = ufs.find(e.v);
    for (; u != v; v = ufs.find(fa[v])) {
        if (dep[u] > dep[v]) std::swap(u, v);
        if (++cnt[v] == 2) ufs.unite(v, fa[v]);
    }
}
int main() {
    int n, m, q, tid;
    fin >> n >> m >> q >> tid;
    ufs.init(n);
    for (int u, v, i = 1; i <= m; i++)
        fin >> u >> v, edgs[i] = {u, v};
    for (int i = 1; i <= q; i++) {
        fin >> ops[i].op >> ops[i].x;
        if (ops[i].op == 2) fin >> ops[i].y;
        else del[ops[i].x] = 1;
    }
    for (int i = 1; i <= m; i++)
        if (!del[i]) insedg(edgs[i]);
    for (int i = q; i >= 1; i--)
        if (ops[i].op == 1) insedg(edgs[ops[i].x]);
    for (int i = 1; i <= n; i++)
        if (!fa[i]) fa[i] = i, dfs(i);
    ufs.init(n);
    for (int i = 1; i <= m; i++)
        if (!del[i]) coveredg(edgs[i]);
    for (int i = q; i >= 1; i--) {
        if (ops[i].op == 1) coveredg(edgs[ops[i].x]);
        else ans[i] = ufs.find(ops[i].x) == ufs.find(ops[i].y);
    }
    for (int i = 1; i <= q; i++)
        if (ops[i].op == 2)
            puts(ans[i] ? "YES" : "NO");
    return 0;
}