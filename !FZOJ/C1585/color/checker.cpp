#include <bits/stdc++.h>
constexpr int N = 200005;
std::vector<std::pair<int, int>> g[N];
bool col[N];
int main(int argc, char **argv) {
    std::ifstream inf(argv[1]), ouf(argv[2]);
    int T, n, m;
    inf >> T;
    bool ac = true;
    for (int _t = 1; _t <= T; _t++) {
        inf >> n >> m;
        for (int i = 1; i <= n; i++) g[i].clear();
        int tot = 0;
        std::vector<std::pair<int,int>> edgs;
        edgs.reserve(m);
        while (m--) {
            int u, v;
            inf >> u >> v;
            edgs.emplace_back(u, v);
            ++tot;
            g[u].emplace_back(v, tot), g[v].emplace_back(u, tot);
        }
        for (int i = 1; i <= n; i++) ouf >> col[i];
        bool accepted = true;
        for (int i = 1; i <= n; i++)
            for (const auto &[j, id1] : g[i])
                for (const auto &[k, id2] : g[j])
                    if (id1 != id2 && col[i] == col[j] && col[i] == col[k]) {
                        accepted = false;
                        break;
                    }
        if (!accepted) {
            std::cout << "Wrong Answer on Test Case " << _t << "! ";
            std::cout << "Edges(" << edgs.size() << "): ";
            for (const auto &[u, v] : edgs)
                std::cout << '<' << u << ',' << v << '>' << ' ';
            std::cout << "Your answer: ";
            for (int i = 1; i <= n; i++)
                std::cout << (int)(col[i]);
            std::cout << '\n';
            ac = false;
        }
    }
    if (ac) std::cout << "Accepted!\n";
    else return 1;
    return 0;
}