#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <unordered_set>
using ULL = unsigned long long;
unsigned long total_virtual_kb(){
    FILE *f; char buf[256]; unsigned long v=0, tot=0;
    if((f=fopen("/proc/self/smaps_rollup","r"))){
        while(fgets(buf,sizeof(buf),f)) if(strncmp(buf,"Size:",5)==0){ sscanf(buf,"Size: %lu kB",&v); fclose(f); return v; }
        fclose(f);
    }
    if((f=fopen("/proc/self/smaps","r"))){
        while(fgets(buf,sizeof(buf),f)) if(strncmp(buf,"Size:",5)==0){ sscanf(buf,"Size: %lu kB",&v); tot+=v; }
        fclose(f);
    }
    return tot;
}
namespace FastI {
    char buf[1 << 22], *p1=nullptr, *p2=nullptr;
    inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = ch ^ '0', ch = gc(); ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
static char obuf[1 << 20];
struct FastO {
    char *p3;
    const char* p4;
    inline FastO() : p3(obuf), p4(obuf + (sizeof obuf)) {}
    inline ~FastO() { fwrite(obuf, p3-obuf, 1, stdout); }
    inline void put(char ch) {
        if (p3 == p4) fwrite(obuf, sizeof obuf, 1, stdout), p3 = obuf;
        *p3++ = ch;
    }
    inline void operator()(ULL x) {
        static char sta[20];
        sta[sta[0] = 1] = x % 10 | '0';
        while (x /= 10) sta[++sta[0]] = x % 10 | '0';
        while (sta[sta[0]]) put(sta[sta[0]--]);
    }
    inline void operator()(char ch) { put(ch); }
} out;
constexpr int N = 500005;
using pii_t = std::pair<int, int>;
using pli_t = std::pair<ULL, int>;

std::unordered_set<int> ban[N];
struct GraphEdge { int id, nxt; } e[N];
pii_t edgs[N];
int head[N];
inline void addedg(int fr, int id) {
    e[++head[0]] = GraphEdge {id, head[fr]};
    head[fr] = head[0];
}

ULL dis[N];
int n;
ULL ans[N];
bool vis[N];
inline void dij() {
    std::priority_queue<pli_t, std::vector<pli_t>, std::greater<pli_t>> pq;
    memset(dis, 0x3f, sizeof dis);
    memset(vis, 0, sizeof vis);
    edgs[0] = {1, 0};
    pq.emplace(0ULL, 0); dis[0] = 0ULL;
    while (!pq.empty()) {
        auto [curw, fr] = pq.top(); pq.pop();
        if (vis[fr]) continue;
        vis[fr] = true;
        int u = edgs[fr].first;
        for (int i = head[u]; i; i = e[i].nxt) {
            auto [v, w] = edgs[e[i].id];
            if (!ban[i].count(fr) && dis[i] > curw + w) {
                dis[i] = curw + w;
                pq.emplace(dis[i], i);
            }
        }
    }
}
int main() {
    freopen("SSSP.in", "r", stdin);
    freopen("SSSP.out", "w", stdout);
    int T, Id, m;
    in(T), in(Id);
    while (T--) {
        memset(head, 0, sizeof head);
        in(n), in(m);
        for (int u, v, w, i=1; i <= m; ++i) {
            in(u), in(v), in(w);
            addedg(u, i);
            edgs[i] = pii_t(v, w);
        }
        for (int i = 1, k, t; i <= m; i++) {
            in(k);
            ban[i].clear();
            ban[i].reserve(k << 1 | 1);
            while (k--) {
                in(t);
                ban[i].insert(t);
            }
        }
        dij();
        memset(ans, 0x3f, sizeof ans);
        for (int i = 0; i <= m; i++)
            ans[edgs[i].first] = std::min(ans[edgs[i].first], dis[i]);
        for (int i = 1; i <= n; i++) {
            if (ans[i] ^ 0x3f3f3f3f3f3f3f3fULL) out(ans[i]), out(' ');
            else out('-'), out('1'), out(' ');
        }
        out('\n');
    }
#ifndef ONLINE_JUDGE
    fprintf(stderr, "Total Mem: %zu\n", total_virtual_kb());
#endif
    return 0;
}