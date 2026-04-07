#include <bits/stdc++.h>
#include "tree.h"
std::vector<std::pair<int,int>> solve(int n) {
    const int Limit = n <= 2000 ? 14 : 20;
    std::vector<int> quest(n-1);
    std::list<std::pair<int, std::vector<int>>> respool;
    std::vector<std::vector<decltype(respool)::iterator>> belongs(n, std::decay_t<decltype(belongs.front())>(Limit));
    std::vector<int> fa(n, -1), deg(n);
    for (int i = 0, now = 0; i < n-1; i++) {
        do ++now; while (__builtin_popcount(now) != Limit / 2);
        quest[i] = now;
    }
    for (int q = 0; q < Limit; q++) {
        std::vector<int> ask(n-1);
        for (int i = 0; i < n-1; i++)
            ask[i] = quest[i] >> q & 1;
        auto res = query(ask);
        for (const auto &vec : res) {
            respool.emplace_back((int)vec.size(), vec);
            for (int u : vec) belongs[u][q] = std::prev(respool.end());
            if (vec.size() == 1) ++deg[vec.front()];
        }
    }
    std::queue<int> que;
    std::vector<bool> vis(n);
    for (int i = 0; i < n; i++)
        if (deg[i] == Limit/2) que.push(i), vis[i] = true;
    while (!que.empty()) {
        int u = que.front(); que.pop();
        for (auto it : belongs[u]) {
            if (it->first == 1) { // 为父亲，将其孩子都连接上
                for (int v : it->second) if (u != v && fa[v] == -1) fa[v] = u;
            }
            else if (--it->first == 1) {
                int rt = -1;
                for (int v : it->second)
                    if (!vis[v]) { rt = v; break; }
                if (rt != -1) if (++deg[rt] == Limit / 2) que.push(rt), vis[rt] = true;
            }
        }
    }
    std::vector<std::pair<int,int>> edgs; edgs.reserve(n-1);
    for (int i = 0; i < n; i++)
        if (~fa[i]) edgs.emplace_back(fa[i], i);
    return edgs;
}
