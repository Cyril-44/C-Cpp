#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
#include <numeric>
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
    std::vector<int> all;
    for (int p = 1; p <= n; p++) {
        while (vis[p] != NONE) ++p;
        if (p <= n)  all.push_back(p);
    }
    int totmx = -1;
    std::vector<int> fin;
    do {
        for (int i = 0; i < allm.size(); i++) {
            qmods[all[i]] = allm[i];
            vis[all[i]] = MOD;
        }
        for (int i = allm.size(); i < allm.size() + alla.size(); i++) {
            qasks[all[i]] = alla[i - allm.size()];
            vis[all[i]] = ASK;
        }
        int ans = 0;
        memset(multiply, 0, sizeof(int) * (m+1));
        for (int i = 1; i <= n; i++)
            if (vis[i] == MOD)
                update(qmods[i], 1);
            else if (vis[i] == ASK)
                ans += calc(qasks[i]);
            else throw;
        if (ans > totmx) totmx = ans, fin = all;
    } while (std::next_permutation(all.begin(), all.end()));
    printf("%d\n", totmx);
    for (int i = 0; i < allm.size(); i++) {
        qmods[fin[i]] = allm[i];
        vis[fin[i]] = MOD;
    }
    for (int i = allm.size(); i < allm.size() + alla.size(); i++) {
        qasks[fin[i]] = alla[i - allm.size()];
        vis[fin[i]] = ASK;
    }
    int ans = 0;
    memset(multiply, 0, sizeof(int) * (m+1));
    for (int i = 1; i <= n; i++)
        if (vis[i] == MOD)
            update(qmods[i], 1);
        else if (vis[i] == ASK)
            ans += calc(qasks[i]);
        else throw;
    for (int i = 1; i <= n; i++) {
        if (vis[i] == MOD) {
            auto [l, r, w] = qmods[i];
            fprintf(stderr, "%d %d %d\n", l, r, w);
        } else {
            fprintf(stderr, "%d %d\n", qasks[i].first, qasks[i].second);
        }
    }
    return 0;
}