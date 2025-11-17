#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <set>
#include <vector>
constexpr int N = 1005;
std::vector<int> g[N];
int bcccnt[N], bl2[N], top;
namespace Tarjan {
    unsigned low[N], dfn[N], ts;
    int sta[N]; bool insta[N];
    int root;
    void dfs(int u) {
        low[u] = dfn[u] = ++ts;
        sta[++sta[0]] = u;
        insta[u] = true;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    ++top;
                    do {
                #define now sta[sta[0]]
                        bl2[now] = top;
                        ++bcccnt[top];
                        insta[now] = false;
                #undef now
                    } while (sta[sta[0]--] != v);
                }
            }
            else low[u] = std::min(low[u], dfn[v]);
        }
        if (u == root && g[u].size() == 0) {
            ++top;
            bl2[top] = u;
            --sta[0];
            insta[u] = false;
            ++bcccnt[top];
        }
    }
    inline void tarj(int n) {
        memset(bcccnt, 0, sizeof(int) * (top+1));
        memset(dfn, 0, sizeof(int) * (n+1));
        top = 0;
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) root = i, dfs(i);
    }
}
int deg[N];
int main() {
    for (int n, testcase=1; scanf("%d", &n) && n; ++testcase) {
        for (int i = 1; i <= 1000; i++) g[i].clear();
        int mx = 0;
        for (int u, v; n--; ) {
            scanf("%d%d", &u, &v);
            g[u].push_back(v);
            g[v].push_back(u);
            mx = std::max(mx, std::max(u, v));
        }
        n = mx;
        Tarjan::tarj(n);
        memset(deg, 0, sizeof(int) * (n + 1));
        std::set<std::pair<int,int>> st;
        for (int i = 1; i <= n; i++)
            for (int j : g[i])
                if (bl2[i] ^ bl2[j]) {
                    st.emplace(bl2[i], bl2[j]);
                    st.emplace(bl2[j], bl2[i]);
                }
        for (const auto& [i, j] : st)
            ++deg[j]; 
        unsigned long long ans2 = 1;
        int ans1 = 0;
        for (int i = 1; i <= top; i++) {
            if (deg[i] <= 1) ans2 *= bcccnt[i], ;
        }
        if (top == 1) ++ans1, ++ans2;
        printf("Case %d: %d %llu\n", testcase, ans1, ans2);
    }
    return 0;
}
