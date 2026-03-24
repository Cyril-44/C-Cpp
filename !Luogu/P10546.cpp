#include <cstdio>
#include <cstdint>
#include <random>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <unordered_map>
constexpr int N = 10005, Q = 50;
constexpr uint64_t Mask = (1ull << Q) - 1;
uint64_t fq[N];
int f[N][Q], g[N][Q]; // g[u][i] 表示在第 i 组询问中，u 目前有多少可达点。
int edgs[N][2], cnt[N];
/*
经典 trick：从叶子开始往上拨。
经过特殊构造，恰好一定有一半的可达点数量为 1 的那些点就是叶子。其他非叶子为 1 的数量一定 < 一半。
将这些成立的点组成一个掩码。如果能和询问的掩码相等或者组成互补，那就应该是当前的那一条边。
现在考虑咋去找叶子连的点，那必要条件就是当 fv=fu+gu（u-->v）时，u 是 v 的父亲。而如果当精确的一半满足这个条件，那么大概率就是这样的。
同理可以弱化上述的条件，即有一半可达点数量为 gu 的那些点就是叶子。
*/
int main() {
    std::mt19937 rng(std::random_device{}());
    auto getrnd = [&]() -> uint64_t {
        static int a[Q];
        for (int i = 0; i < Q; i++) a[i] = i;
        std::shuffle(a, a+Q, rng);
        uint64_t res = 0;
        for (int i = 0; i < Q/2; i++) res |= 1ull << a[i];
        return res;
    };

    std::unordered_map<uint64_t, int> mp;
    int n;
    { // Input
        scanf("%d", &n);
        for (int i = 1; i < n; i++) {
            do fq[i] = getrnd();
            while (mp.count(fq[i]) || mp.count(fq[i] ^ Mask));
            mp[fq[i]] = mp[fq[i] ^ Mask] = i;
        }
        const auto &P = putchar;
        for (int q = 0; q < Q; q++) {
            P('?'), P(' ');
            for (int i = 1; i < n; i++) P(fq[i] >> q & 1 | '0');
            P('\n'); fflush(stdout);
            for (int i = 1; i <= n; i++) scanf("%d", &f[i][q]);
        }
    }

    for (int i = 1; i <= n; i++)
        for (int j = 0; j < Q; j++)
            g[i][j] = 1;
    static int que[N];
    int hd=0, tl=0;
    std::bitset<N> vis;
    for (int u = 2; u <= n; u++) // 先找到所有的叶子（强制钦定 1 为根）
        if (std::accumulate(f[u], f[u]+Q, 0, [](int acc, int x){ return acc + (x == 1); }) == Q/2)
            que[tl++] = u, vis.set(u);
    while (hd ^ tl) {
        int ttl = tl;
        for (int h = hd, t = tl; h ^ t; ++h) {
            int u = que[h];
            for (int v = 1; v <= n; v++) {
                if (vis[v]) continue;
                for (int i = 0; i < Q; i++) {
                    
                }
            }
        }
    }
    return 0;
}