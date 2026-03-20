#include <bits/stdc++.h>
#include <cassert>
#include <cstdio>
#ifdef ONLINE_JUDGE
# include "grader.h"
#else
void solve(int);
namespace MyGrader{
constexpr int N = 300, MX = 4000;
int a[N], n, qcnt;
inline int query(const std::vector<int>& vec) {
    int idx = 0, cnt = 0;
    for (int i : vec) if (a[idx++] == i) ++cnt;
    if (++qcnt > MX) puts("\e[35;1mQuery Limit Exceeded!\e[0m"), exit(1);
    if (cnt == n) puts("\e[32;1mAccepted!\e[0m"), fprintf(stderr, "Queries count: %d\n", qcnt), exit(0);
    return cnt;
}
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    solve(n);
    puts("\e[31;1mWrong Answer!\e[0m");
    return 2;
}
}
int main() { return MyGrader::main(); }
using MyGrader::query;
#endif
// 考虑一个错排与正确排列可以建 n 条边。即，当前下标到正确下标。易于发现这是多个排列环。
// 需要一个错排，以正确查询出所有边。一次询问可以查询 n/2 条边，但是因为这个边是双向的，所以还需要一次额外的查询才可以知道方向。即最后查询环的时候再做一次判断。
// 最终：2nlogn + n/2 + ?
namespace My{
int n;
using Swaps = std::vector<std::pair<int,int>>;
Swaps ans;
std::vector<int> perm;
int query(const std::vector<int> &vec) {
    int ret = ::query(vec);
    if (ret == n) exit(0);
    return ret;
}
inline int dquery(Swaps::iterator l, Swaps::iterator r) {
    if (l == r) return 0;
    for (auto it = l; it != r; ++it) std::swap(perm[it->first], perm[it->second]);
    int ret = query(perm);
    for (auto it = l; it != r; ++it) std::swap(perm[it->first], perm[it->second]);
    return ret;
}
void dfs(int all, Swaps::iterator l, Swaps::iterator r) {
    if (r - l == 1 && all) return void(ans.push_back(*l));
    auto mid = l + (r-l)/2;
    int lres = dquery(l, mid), rres = all - lres;
    if (lres > 0) dfs(lres, l, mid);
    if (rres > 0) dfs(rres, mid, r);
}
void solve(int n) {
    My::n = n;
    std::mt19937 rng(20100709);
    
    perm.resize(n);
    std::iota(perm.begin(), perm.end(), 1);
    do std::shuffle(perm.begin(), perm.end(), rng);
    while (query(perm));
    
    std::vector<std::pair<int,int>> ranges; ranges.reserve(n / 2 + 1);
    ans.clear();
    for (int w = 0; w < n; w++) { // 通过 i + j \equiv w \bmod n 得到每次要判断的 n/2 条边（不会相互影响）
        ranges.clear();
        for (int i = 0; i < n; i++) {
            int j = w + (i > w) * n - i;
            if (i < j) ranges.emplace_back(i, j);
        }
        dfs(dquery(ranges.begin(), ranges.end()), ranges.begin(), ranges.end());
    }
    // assert(ans.size() == n); 不一定，如果是二元环，那就少了一条边
    
    std::vector<std::vector<int>> g(n); for (int i = 0; i < n; i++) g[i].reserve(2);
    for (const auto &[u, v] : ans) g[u].push_back(v), g[v].push_back(u);
    std::vector<bool> vis(n);
    std::vector<int> ring; ring.reserve(n);
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        ring.clear();
        vis[i] = true; ring.push_back(i);
        for (int j = g[i][0];;) {
            ring.push_back(j), vis[j] = true;
            if (vis[g[j][0]])
                if (g[j].size() == 1 || vis[g[j][1]]) break;
                else j = g[j][1];
            else j = g[j][0];
        }
        cnt += ring.size();
        
        int pfront = perm[ring.front()];
        for (int j = 1; j < (int)ring.size(); j++)
            perm[ring[j - 1]] = perm[ring[j]];
        perm[ring.back()] = pfront;
        if (query(perm) != cnt) {
            for (int _=2; _--; ) {
                int pback = perm[ring.back()];
                for (int j = (int)ring.size() - 1; j >= 1; j--)
                    perm[ring[j]] = perm[ring[j - 1]];
                perm[ring.front()] = pback;
            }
            // assert(query(perm) == cnt);
        }
    }
    query(perm);
}
}
void solve(int n) { My::solve(n); }