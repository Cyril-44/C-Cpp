#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
constexpr int N = 205, M = 1005;
std::tuple<int,int,int> qmods[N];
std::pair<int,int> qasks[N];
enum Type { NONE, MOD, ASK } vis[M];
int multiply[M];
inline int calc(const std::pair<int,int> &p) {
    int res = 0;
    for (int i = p.first; i <= p.second; i++)
        res += multiply[i];
    return res;
}
inline int calc(const std::tuple<int,int,int> &t) {
    return calc({std::get<0>(t), std::get<1>(t)}) * std::get<2>(t);
}
inline void update(const std::pair<int,int> &p, int mult) {
    for (int i = p.first; i <= p.second; i++)
        multiply[i] += mult;
}
inline void update(const std::tuple<int,int,int> &t, int mult) {
    for (int i = std::get<0>(t); i <= std::get<1>(t); i++)
        multiply[i] += mult * std::get<2>(t);
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
    for (int i = 1; i <= n; i++)
        if (vis[i] == ASK)
            update(qasks[i], 1);
    for (const auto &pair : alla)
        update(pair, 1);
    for (int p = 1; !allm.empty(); p++) {
        for (; vis[p] != NONE; p++)
            if (vis[p] == ASK)
                update(qasks[p], -1);
        int curmx = calc(allm.front()); auto curmxp = allm.begin();
        for (auto it = ++allm.begin(); it != allm.end(); ++it) {
            int res = calc(*it);
            if (res > curmx) curmx = res, curmxp = it;
        }
        qmods[p] = *curmxp;
        vis[p] = MOD;
        allm.erase(curmxp);
    }
    memset(multiply, 0, sizeof(int) * (m+1));
    for (int i = 1; i <= n; i++)
        if (vis[i] == MOD)
            update(qmods[i], 1);
    for (int p = n; !alla.empty(); --p) {
        for (; vis[p] != NONE; --p)
            if (vis[p] == MOD)
                update(qmods[p], -1);
        int curmx = calc(alla.front()); auto curmxp = alla.begin();
        for (auto it = ++alla.begin(); it != alla.end(); ++it) {
            int res = calc(*it);
            if (res > curmx) curmx = res, curmxp = it;
        }
        qasks[p] = *curmxp;
        vis[p] = ASK;
        alla.erase(curmxp);
    }
    memset(multiply, 0, sizeof(int) * (m+1));
    int ans = 0;
    assert(alla.empty() && allm.empty());
    for (int i = 1; i <= n; i++)
        if (vis[i] == MOD)
            update(qmods[i], 1);
        else if (vis[i] == ASK)
            ans += calc(qasks[i]);
        else throw;
    printf("%d\n", ans);
    return 0;
}