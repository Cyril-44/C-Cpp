#include "tree.h"
#include <numeric>
#include <queue>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <cassert>
#include <random>
std::vector<std::pair<int,int>> solve(int n) {
    std::mt19937 rng(std::random_device{}());
    const int limit = (n <= 2000) ? 14 : 20;
    using Block = std::unordered_set<int>;
    using BlockID = std::list<Block>::iterator;

    std::vector<unsigned> qvec; qvec.reserve(1 << limit); // 询问的 01 数组
    std::vector<int> mp(1 << limit); // 询问对应的边
    std::list<Block> allBlocks; // 得到的所有连通块存储池
    std::vector<std::vector<BlockID>> qf(n, std::vector<BlockID>(limit)); // qf[i][j] 表示第 i 条边在第 j 次询问中所属的连通块 ID

    for (int cur = 0; !(cur >> limit & 1); ) {
        while (!(cur >> limit & 1) && __builtin_popcount(++cur) != limit / 2); // 让询问中刚好一半的边启用（经典 Trick）
        qvec.push_back(cur);
    }
    std::shuffle(qvec.begin(), qvec.end(), rng);
    qvec.resize(n-1);

    for (int q = 0; q < limit; q++) {
        std::vector<int> ask; ask.reserve(n-1);
        for (int i = 0; i < n-1; i++)
            ask.push_back(qvec[i] >> q & 1);
        auto res = query(ask);
        for (const auto &block : res) {
            allBlocks.emplace_back(block.begin(), block.end());
            for (int u : block) qf[u][q] = std::prev(allBlocks.end());
        }
    }
    
    std::vector<std::pair<int,int>> edgs; edgs.reserve(n-1); // 最终的答案边数组
    std::vector<int> cnt(n);  // 这个点当前连通块大小为 1 的询问个数
    std::vector<bool> vis(n); // 这个点有没有做完
    std::queue<int> que;
    auto push = [&](int u) { if (!vis[u]) vis[u] = true, que.push(u); };
    for (int i = 1; i < n; i++) { // 钦定 0 不为叶子
        cnt[i] = std::accumulate(qf[i].begin(), qf[i].end(), 0, [](int acc, const BlockID &x) { return acc + (x->size() == 1); });
        if (cnt[i] == limit/2) push(i);
    }
    while (!que.empty()) {
        int u = que.front(); que.pop();

        std::vector<int> order; order.reserve(limit/2);
        for (int q = 0; q < limit; q++) // 找到当前连通块大小还 >= 1 的询问
            if (qf[u][q]->size() > 1) order.push_back(q);
        if (order.empty()) break;
        std::shuffle(order.begin(), order.end(), rng);
        auto now = *qf[u][order.back()]; order.pop_back();
        
        for (auto it = now.begin(); it != now.end(); ) // 要先用 vis 去重
            (vis[*it]) ? (it = now.erase(it)) : (++it);
        for (int q : order)
            for (auto it = now.begin(); it != now.end(); ) // 求并集
                (qf[u][q]->count(*it)) ? (++it) : (it = now.erase(it));
        
        assert(now.size() == 1);
        edgs.emplace_back(u, *now.begin()); // 加入 <u, fa>

        for (int q = 0; q < limit; q++) { // 更新状态，找到当前是叶子的节点
            qf[u][q]->erase(u);
            if (qf[u][q]->size() == 1) if (++cnt[*qf[u][q]->begin()] == limit/2) push(*qf[u][q]->begin());
        }
    }
    return edgs;
}
