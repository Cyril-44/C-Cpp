#include <bits/stdc++.h>
struct FastI{char buf[1<<20],*p1{},*p2{};[[gnu::always_inline]]char gc(){if(p1==p2)p2=(p1=buf)+fread(buf,1,sizeof buf,stdin);if(p1==p2)return EOF;return*p1++;}template<class T>void operator()(T&x){char ch=gc();while(ch<'0'||ch>'9')ch=gc();for(x=0;ch>='0'&&ch<='9';ch=gc())x=(x<<3)+(x<<1)+(ch^'0');}}in;struct FastO{char buf[1<<20],*p1{buf};const char*p2{buf+sizeof(buf)};[[gnu::always_inline]]void pc(char ch){if(p1==p2)fwrite(buf,sizeof buf,1,stdout),p1=buf;*p1++=ch;}void flush(){fwrite(buf,p1-buf,1,stdout),p1=buf;}template<class T>void operator()(T x){static uint8_t sta[64];sta[sta[0]=1]=x%10|'0';while(x/=10)sta[++sta[0]]=x%10|'0';while(sta[0])pc(sta[sta[0]--]);}~FastO(){flush();}}out;
#ifdef CLANGD
constexpr int N = 24;
#else
constexpr int N = 200004;
#endif
using Pii = std::pair<int,int>;
std::vector<Pii> g[N];
int t[N], cruc[N], n; // cruc[i] 表示 点 i 到关键点路径上的边权的最大值 的最小值
struct Edge {
    int u, v, w;
    bool operator<(const Edge& e) const { return w < e.w; }  
} edgs[N];
struct UFS {
    int fa[N], sz[N][2];
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    void unite(int u, int v) { // Assume that u and v are root and hasn't united
        fa[u] = v, sz[v][0] += sz[u][0], sz[v][1] += sz[u][1];
    }
    void init() { for (int i = 1; i <= n; i++) fa[i] = i, sz[i][0] = !t[i], sz[i][1] = t[i]; }
} uf;
int main() {
    int T, k; in(T);
    while (T--) {
        in(n), in(k);
        for (int i = 1; i <= n; i++) g[i].clear(), in(t[i]);
        for (int i = 1, u, v, w; i < n; i++) {
            in(u), in(v), in(w);
            g[u].emplace_back(v, w);
            g[v].emplace_back(u, w);
            edgs[i] = {u, v, w};
        }
        {
            static bool vis[N];
            memset(vis, 0, n+1);
            std::priority_queue<Pii, std::vector<Pii>, std::greater<Pii>> pq;
            for (int id; k--; )
                in(id), pq.emplace(0, id);
            while (!pq.empty()) {
                auto [cu, u] = pq.top(); pq.pop();
                if (vis[u]) continue;
                vis[u] = true, cruc[u] = cu;
                for (auto [v, w] : g[u]) if (!vis[v])
                    pq.emplace(std::max(cu, w), v);
            }
            // for (int i = 1; i <= n; i++)
            //     fprintf(stderr, "%d%c", cruc[i], " \n"[i==n]);
            for (int i = 1; i < n; i++)
                edgs[i].w = std::max({edgs[i].w, cruc[edgs[i].u], cruc[edgs[i].v]});
        }
        std::sort(edgs+1, edgs+n);
        uint64_t ans = 0;
        uf.init();
        static std::tuple<int,int,int,int,int,int,int> hist[N];
        for (int i = 1; i < n; i++) {
            int u = uf.find(edgs[i].u), v = uf.find(edgs[i].v);
            hist[i] = {u, v, uf.sz[u][0], uf.sz[u][1], uf.sz[v][0], uf.sz[v][1], edgs[i].w};
            ans += 1ull * edgs[i].w * (1ull * uf.sz[u][0] * uf.sz[v][1] + 1ull * uf.sz[u][1] * uf.sz[v][0]);
            uf.unite(u, v);
        }
        static uint64_t val[N][2]; constexpr int VSZ = sizeof(uint64_t) * 2;
        memset(val, 0, VSZ * (n+1));
        for (int i = n-1; i >= 1; i--) {
            int u, v, szu[2], szv[2], w;
            std::tie(u, v, szu[0], szu[1], szv[0], szv[1], w) = hist[i];
            memcpy(val[u], val[v], VSZ);
            val[u][0] += 1ull * w * szv[0];
            val[u][1] += 1ull * w * szv[1];
            val[v][0] += 1ull * w * szu[0];
            val[v][1] += 1ull * w * szu[1];
        }
        // for (int i = 1; i <= n; i++)
        //     fprintf(stderr, "%llu %llu\n", val[i][0], val[i][1]);
        out(ans), out.pc('\n');
        for (int i = 1; i <= n; i++)
            out(t[i] ? ans - val[i][0] + val[i][1] : ans - val[i][1] + val[i][0]), out.pc('\n');
    }
    return 0;
}