#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
#include <numeric>
#include <random>
constexpr int N = 205, M = 1005;
std::tuple<int,int,int> qmods[N];
std::pair<int,int> qasks[N];
enum Type { NONE, MOD, ASK } vis[M];
int sum[N][M];
int multiply[M];
inline int calc(int i, const std::tuple<int,int,int> &t) {
    return std::get<2>(t) * (sum[i][std::get<1>(t)] - sum[i][std::get<0>(t)-1]);
}
inline int calc(int i, const std::pair<int,int> &t) {
    return sum[i][t.second] - sum[i][t.first-1];
}
inline void update(const std::tuple<int,int,int> &t) {
    for (int i = std::get<0>(t); i <= std::get<1>(t); i++)
        multiply[i] += std::get<2>(t);
}
inline int calc(const std::pair<int,int> &p) {
    int res = 0;
    for (int i = p.first; i <= p.second; i++)
        res += multiply[i];
    return res;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<std::tuple<int,int,int>> allm;
    std::vector<std::pair<int,int>> alla;
    for (int i = 1; i <= n; i++) {
        int type, l, r, w, pos;
        scanf("%d%d%d", &type, &l, &r);
        if (type == 1) scanf("%d", &w);
        scanf("%d", &pos);
        if (pos) {
            vis[pos] = Type(type);
            if (type == 1) qmods[pos] = {l, r, w};
            else qasks[pos] = {l, r};
        } else {
            if (type == 1) allm.emplace_back(l, r, w);
            else alla.emplace_back(l, r);
        }
    }

    std::mt19937 rng(std::random_device{}());
    std::shuffle(allm.begin(), allm.end(), rng);
    std::shuffle(alla.begin(), alla.end(), rng);

    std::vector<int> allem;
    for (int i = 1; i <= n; i++)
        if (vis[i] == NONE) allem.push_back(i);

    for (const auto &asks : alla)
        for (int j = asks.first; j <= asks.second; j++) ++sum[n+1][j];
    for (int i = n; i >= 1; i--) {
        memcpy(sum[i], sum[i+1], sizeof(int) * (m+1));
        if (vis[i] == ASK)
            for (int j = qasks[i].first; j <= qasks[i].second; j++) ++sum[i][j];
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++)
            sum[i][j] += sum[i][j-1];
    }
    for (int i = 1; i < (int)allm.size(); i++) {
        for (int j = (int)allm.size() - 1; j >= i; j--)
            if (calc(allem[j-1], allm[j-1]) + calc(allem[j], allm[j]) < calc(allem[j], allm[j-1]) + calc(allem[j-1], allm[j]))
                std::swap(allm[j], allm[j-1]);
    }
    for (int i = 0; i < (int)allm.size(); i++)
        vis[allem[i]] = MOD, qmods[allem[i]] = allm[i];
    
    allem.erase(allem.begin(), allem.begin() + allm.size());
    memset(sum, 0, sizeof sum);
    
    for (const auto &mods : allm) {
        int l, r, w;
        std::tie(l, r, w) = mods;
        for (int j = l; j <= r; j++) ++sum[0][j];
    }
    for (int i = 1; i <= n; i++) {
        memcpy(sum[i], sum[i-1], sizeof(int) * (m+1));
        if (vis[i] == MOD) {
            int l, r, w;
            std::tie(l, r, w) = qmods[i];
            for (int j = l; j <= r; j++) ++sum[i][j];
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++)
            sum[i][j] += sum[i][j-1];
    }
    for (int i = 1; i < (int)alla.size(); i++) {
        for (int j = (int)alla.size() - 1; j >= i; j--)
            if (calc(allem[j-1], alla[j-1]) + calc(allem[j], alla[j]) < calc(allem[j], alla[j-1]) + calc(allem[j-1], alla[j]))
                std::swap(alla[j], alla[j-1]);
    }
    for (int i = 0; i < (int)alla.size(); i++)
        vis[allem[i]] = ASK, qasks[allem[i]] = alla[i];

    int ans = 0;
    for (int i = 1; i <= n; i++)
        if (vis[i] == MOD)
            update(qmods[i]);
        else if (vis[i] == ASK)
            ans += calc(qasks[i]);
        else throw;
    printf("%d\n", ans);
    return 0;
}