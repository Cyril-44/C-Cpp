#include "tree.h"
#include <numeric>
#include <queue>
#include <unordered_set>
#include <list>
std::vector<std::pair<int,int>> solve(int n) {
    const int limit = (n <= 2000) ? 14 : 20;
    using Block = std::unordered_set<int>;
    using BlockID = std::list<Block>::iterator;

    std::vector<unsigned> qvec(n+1); // 询问的 01 数组
    std::vector<int> mp(1 << limit); // 询问对应的边
    std::list<Block> allBlocks; // 得到的所有连通块存储池
    std::vector<std::vector<BlockID>> qf(n+1, std::vector<BlockID>(limit)); // qf[i][j] 表示第 i 条边在第 j 次询问中所属的连通块 ID

    for (int i = 1, cur = 0; i < n; i++) {
        while (__builtin_popcount(++cur) != limit / 2); // 让询问中刚好一半的边启用（经典 Trick）
        qvec[i] = cur, mp[cur] = i;
    }
    for (int q = 0; q < limit; q++) {
        std::vector<int> ask; ask.reserve(n);
        for (int i = 1; i < n; i++)
            if (qvec[i] >> q & 1) ask.push_back(i);
        auto res = query(ask);
        for (const auto &block : res) {
            allBlocks.emplace_back(block.begin(), block.end());
            for (int u : block) qf[u][q] = std::prev(allBlocks.end());
        }
    }
    
    std::vector<std::pair<int,int>> edgs; edgs.reserve(n); // 最终的答案边数组
    std::vector<int> cnt(n+1);  // 这个点当前连通块大小为 1 的询问个数
    std::vector<bool> vis(n+1); // 这个点有没有做完
    std::queue<int> que;
    for (int i = 1; i <= n; i++) {
        cnt[i] = std::accumulate(qf[i].begin(), qf[i].end(), 0, [](int acc, const BlockID &x) { return acc + x->size() == 1; });
        if (cnt[i] == limit/2) que.push(i), vis[i] = true;
    }
    while (!que.empty()) {
        int u = que.front();
        for (int q = 0; q < limit; q++) {
            
        }
    }
}
