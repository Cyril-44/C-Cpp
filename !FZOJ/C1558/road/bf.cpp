#include <stdio.h>
#include <string.h>
#include <set>
#include <vector>
#include <algorithm>
constexpr int N = 5005;
std::set<int> g[N];
int n, m;
inline int calc(int S) {
    static int dis[N], que[N];
    memset(dis, -1, sizeof(int) * (n+1));
    dis[S] = 0;
    int l = 0, r = 0;
    que[r++] = S;
    int cnt = 1;
    while ((l ^ r) && (cnt ^ n)) {
        int u = que[l++];
        for (int v : g[u])
            if (dis[v] == -1) {
                dis[v] = dis[u] + 1;
                que[r++] = v;
                ++cnt; 
            }
    }
    int res = 0;
    for (int i = 1; i <= n; i++)
        if (dis[i] == -1) return 1145141919;
        else res += dis[i];
    return res;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i < n; i++)
        for (int j = i+1; j <= n; j++)
            g[i].insert(j);
    std::vector<int> best{1};
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        if (g[u].count(v)) g[u].erase(v), g[v].insert(u);
        else g[v].erase(u), g[u].insert(v);
        best.push_back(u), best.push_back(v);
        std::vector<int> cb; cb.reserve(best.size());
        for (int i : best) cb.push_back(calc(i));
        int mn = *std::min_element(cb.begin(), cb.end());
        printf("%d\n", mn);
        std::vector<int> newbest;
        for (int i = 0; i < (int)cb.size(); i++)
            if (cb[i] == mn) newbest.push_back(best[i]);
        best.swap(newbest);
        fprintf(stderr, "Best Size: %zu\n", best.size());
    }
    return 0;
}