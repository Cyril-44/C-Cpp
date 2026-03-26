#include <cstdio>
#include <cstdint>
#include <random>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <stdexcept>
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
注意钦定 1 为根，每次不要以 1 为起点开始找就可以了。
*/
int main() {
    const auto &P = putchar;

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
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        do fq[i] = getrnd();
        while (mp.count(fq[i]) || mp.count(fq[i] ^ Mask));
        mp[fq[i]] = i;
        // fprintf(stderr, "Edge %d: -%lX, %lX\n", i, fq[i], fq[i] ^ Mask);
    }
    for (int q = 0; q < Q; q++) {
        P('?'), P(' ');
        for (int i = 1; i < n; i++) P(fq[i] >> q & 1 | '0');
        P('\n'); fflush(stdout);
        for (int i = 1; i <= n; i++) scanf("%d", &f[i][q]);
    }

    for (int u = 1; u <= n; u++)
        for (int i = 0; i < Q; i++)
            g[u][i] = 1, cnt[u] += f[u][i] == 1;
    static int que[N];
    int hd=0, tl=0;
    std::bitset<N> vis;
    for (int u = 2; u <= n; u++) // 先找到所有的叶子（强制钦定 1 为根）
        if (cnt[u] == Q/2) que[tl++] = u, vis.set(u);
    while (hd != tl) {
        int u = que[hd++];
        uint64_t to = 0; static int onvec[Q/2];
        for (int i = 0, j = 0; i < Q; i++)
            if (f[u][i] > g[u][i]) // 这条边是指向 u 的
                to |= 1ull << i, onvec[j++] = i;
        // fprintf(stderr, "Node %d: %lX\n", u, to);
        int edgid; bool flip; // flip=0 ==> u-->v, flip=1 ==> u<--v
        mp.count(to) ? (edgid = mp[to], flip = false) : (edgid = mp[to ^ Mask], flip = true);
        edgs[edgid][flip] = u;
        for (int v = 1; v <= n; v++) if (!vis[v]) {
            for (int j : onvec) if (f[v][j] + g[u][j] != f[u][j]) goto cont;
            for (int i = 0; i < Q; i++) // 更新 u-->v
                if ((to >> i & 1) ^ 1) {
                    if (g[v][i] == f[v][i]) --cnt[v];
                    g[v][i] += g[u][i];
                    if (g[v][i] == f[v][i]) ++cnt[v];
                }
            edgs[edgid][flip ^ 1] = v;
            if (cnt[v] == Q/2) que[tl++] = v, vis.set(v);
            goto end;
            cont:;
        }
        // throw std::runtime_error("What the fuck is this?");
        end:;
    }
    
    P('!');
    for (int i = 1; i < n; i++)
        printf(" %d %d", edgs[i][0], edgs[i][1]);
    P('\n'); fflush(stdout);
    return 0;
}