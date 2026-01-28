#include <bits/stdc++.h>
using namespace std;

using ll = long long;
std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

ll rnd(ll l, ll r) {
    return std::uniform_int_distribution<ll>(l, r)(rng);
}

struct Edge {
    int u, v;
    ll c;
};

vector<Edge> edges;

void add_edge(int u, int v, ll c) {
    edges.push_back({u, v, c});
}

int main(int argc, char** argv) {
    // 参数：mode seed
    if (argc < 3) {
        cerr << "Usage: ./gen <mode> <seed>\n";
        return 0;
    }
    int mode = atoi(argv[1]);
    rng.seed(atoll(argv[2]));

    int M = 1000;          // 最大点数
    int N = 500000;        // 最大边数
    edges.reserve(N);

    // ============================
    // 1. 极端链式图（最坏 BFS）
    // ============================
    if (mode == 1) {
        for (int i = 1; i < M; i++) {
            add_edge(i, i + 1, rnd(1, 1e7));
        }
        // 填满剩余边（全部指向下一层）
        while ((int)edges.size() < N) {
            int u = rnd(1, M - 1);
            add_edge(u, u + 1, rnd(1, 1e7));
        }
    }

    // ============================
    // 2. 极端星型图（最坏 DFS）
    // ============================
    else if (mode == 2) {
        for (int i = 2; i <= M; i++) {
            add_edge(1, i, rnd(1, 1e7));
        }
        while ((int)edges.size() < N) {
            add_edge(1, rnd(2, M), rnd(1, 1e7));
        }
    }

    // ============================
    // 3. 强连通稠密图（大量环）
    // ============================
    else if (mode == 3) {
        for (int i = 1; i <= M; i++) {
            for (int j = i + 1; j <= M; j++) {
                add_edge(i, j, rnd(1, 1e7));
                if ((int)edges.size() >= N) break;
            }
            if ((int)edges.size() >= N) break;
        }
    }

    // ============================
    // 4. 随机图（均匀分布）
    // ============================
    else if (mode == 4) {
        for (int i = 0; i < N; i++) {
            int u = rnd(1, M);
            int v = rnd(1, M);
            if (u == v) v = (v % M) + 1;
            add_edge(u, v, rnd(0, 10000000));
        }
    }

    // ============================
    // 5. 单瓶颈图（卡 DFS）
    // ============================
    else if (mode == 5) {
        // 大量大容量边
        for (int i = 1; i < M; i++) {
            add_edge(i, i + 1, rnd(5e6, 1e7));
        }
        // 单瓶颈
        add_edge(M - 1, M, 1);

        while ((int)edges.size() < N) {
            int u = rnd(1, M - 1);
            add_edge(u, u + 1, rnd(5e6, 1e7));
        }
    }

    // ============================
    // 6. 混合极端图（最全面）
    // ============================
    else if (mode == 6) {
        // 链
        for (int i = 1; i < M; i++) add_edge(i, i + 1, rnd(1, 1e7));

        // 星
        for (int i = 2; i <= M; i++) add_edge(1, i, rnd(1, 1e7));

        // 随机
        while ((int)edges.size() < N) {
            add_edge(rnd(1, M), rnd(1, M), rnd(0, 1e7));
        }
    }

    // 输出
    cout << edges.size() << " " << M << "\n";
    for (auto &e : edges) {
        cout << e.u << " " << e.v << " " << e.c << "\n";
    }
    return 0;
}
