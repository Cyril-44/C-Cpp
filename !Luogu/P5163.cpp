#include <stdio.h>
#include <algorithm>
#include <tuple>
constexpr int N = 100005;
std::vector<int> g[N];
int s[N];
int dfn[N], low[N], ts, sta[N], pres[N];
void tarjan(int u) {
    static int sta[N];
    dfn[u] = low[u] = ++ts;
    sta[++sta[0]] = u;
    for (int v : g[u]) {
        if (!dfn[v]) tarjan(v), low[u] = std::min(low[u], low[v]);
        else low[u] = std::min(low[u], low[v]);
    }
    if (dfn[u] == low[u]) {
        do pres[sta[sta[0]]] = u;
        while (sta[sta[0]--] != u);
    }
}
using Edge = std::tuple<int,int,int>; // <u,v,time>
using Iter = std::vector<Edge>::iterator;
inline void solve(int l, int r, Iter beg, Iter end) { // 时间在 [l,r] 内的进行处理
    int mid = l + r >> 1;
    std::nth_element(beg, beg+mid, end, [mid](const Edge&x, const Edge&y){return std::get<2>(x) < std::get<2>(y);});
    
}

int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    std::vector<std::pair<int,int>> edgs;
    std::vector<Edge> dels, mods;
    std::vector<std::pair<int,int>> ques;
    edgs.reserve(m);
    for (int i = 1; i <= n; i++) scanf("%d", &s[i]);
    for (int i = 1; i <= m; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        edgs.emplace_back(x, y);
    }
    for (int i = q, opt, x, y; i >= 1; i--) {
        scanf("%d%d%d", &opt, &x, &y);
        if (opt == 1) {
            dels.emplace_back(i, x, y);
        } else if (opt == 2) {
            mods.emplace_back(i, x, y);
        } else {
            ques.emplace_back(i, x, y);
        }
    }

    return 0;
}