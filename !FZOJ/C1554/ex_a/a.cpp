#include <stdio.h>
#include <cstring>
#include <vector>
#include <algorithm>
constexpr int N = 2005;
using LL = long long;
struct Edge { int to, nxt, w; bool flg; } e[N << 2];
int head[N];
inline void addedg(int fr, int to, int w) {
    e[++head[0]] = Edge{to, head[fr], w, false};
    head[fr] = head[0];
}
struct DpF {
    LL w;
    int id;
    inline bool operator< (const DpF &rhs) const { return w < rhs.w; }
} f[N][2];
int mxptr;
LL mxdist;
void getdiam(int u, int pre) {
    f[u][0] = {0, 0}, f[u][1] = {0, 0};
    for (int i = head[u], v; i; i = e[i].nxt) {
        if (e[i].flg || (v = e[i].to) == pre) continue;
        getdiam(v, u);
        DpF v0{f[v][0].w + e[i].w, v}, v1{f[v][1].w + e[i].w, v};
        if (f[u][0] < v0) f[u][1] = f[u][0], f[u][0] = v0;
        else if (f[u][1] < v0) f[u][1] = v0;
    }
    if (f[u][0].w + f[u][1].w > mxdist) {
        mxdist = f[u][0].w + f[u][1].w;
        mxptr = u;
    }
}
bool isindiam[N];
std::vector<int> diams;
void extrdiam() {
    mxptr = 0; mxdist = 0;
    getdiam(1, 0);
    diams.clear();
    for (int i = f[mxptr][0].id; i; i = f[i][0].id)
        diams.push_back(i);
    std::reverse(diams.begin(), diams.end());
    diams.push_back(mxptr);
    for (int i = f[mxptr][1].id; i; i = f[i][0].id)
        diams.push_back(i);
    memset(isindiam, 0, sizeof isindiam);
    for (int i : diams) isindiam[i] = true;
}
LL mx;
int mxvtx;
void dfs(int u, int pre, LL val) {
    if (val > mx) mx = val, mxvtx = u;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if (e[i].flg || (v = e[i].to) == pre) continue;
        dfs(v, u, val + e[i].w);
    }
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        addedg(u, v, w); addedg(v, u, w);
    }
    for (int times = 0; times <= k; times++) {
        extrdiam();
        printf("%lld\n", mxdist);
        if (diams.size() == n) continue;
        int mxi = -1, allmxvtx = -1;
        LL allmx = 0;
        for (int i = 1; i < (int)diams.size(); i++) {
            mx = 0, mxvtx = -1;
            for (int j = head[diams[i]]; j; j = e[j].nxt)
                if (!e[j].flg && !isindiam[e[j].to]) dfs(e[j].to, diams[i], e[j].w);
            if (mx > allmx) allmx = mx, mxi = i, allmxvtx = mxvtx;
        }
        int v = diams[mxi], u = diams[mxi - 1], w = -1;
        for (int i = head[u]; i; i = e[i].nxt)
            if (e[i].to == v) {e[i].flg = true; w = e[i].w; break;}
        for (int i = head[v]; i; i = e[i].nxt)
            if (e[i].to == u) {e[i].flg = true; break;}
        addedg(allmxvtx, u, w);
        addedg(u, allmxvtx, w);
    }
    return 0;
}