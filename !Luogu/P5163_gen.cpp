#include <bits/stdc++.h>
using namespace std;

/*
用法示例（命令行）：
./gen n m q maxS seed

例如：
./gen 5 8 8 10 1

若不传参数，则使用代码中的默认值。
*/

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 默认参数
    int n = 10;          // 点数
    int m = 20;          // 边数
    int q = 30;          // 操作数
    long long maxS = 100; // 初始发达程度最大值
    unsigned int seed = chrono::steady_clock::now().time_since_epoch().count();

    if (argc >= 2) n = atoi(argv[1]);
    if (argc >= 3) m = atoi(argv[2]);
    if (argc >= 4) q = atoi(argv[3]);
    if (argc >= 5) maxS = atoll(argv[4]);
    if (argc >= 6) seed = (unsigned int)strtoul(argv[5], nullptr, 10);

    mt19937_64 rng(seed);
    auto rndInt = [&](long long L, long long R) {
        return (long long)(rng() % (R - L + 1) + L);
    };

    // 保证 m 不超过 n*(n-1)
    long long maxPossibleEdges = 1LL * n * (n - 1);
    if (m > maxPossibleEdges) m = (int)maxPossibleEdges;
    if (m < 0) m = 0;
    if (q < 0) q = 0;

    // 生成初始发达程度 s_i
    vector<long long> s(n + 1);
    for (int i = 1; i <= n; ++i) {
        s[i] = rndInt(1, maxS);
    }

    // 生成无重边有向图
    // 用一个 set 存 (u,v) 防止重边
    vector<pair<int,int>> edges;
    edges.reserve(m);
    {
        unordered_set<long long> used;
        used.reserve(m * 2 + 10);
        auto encode = [&](int u, int v) {
            return ( (long long)u << 32 ) ^ (unsigned long long)v;
        };

        while ((int)edges.size() < m) {
            int u = (int)rndInt(1, n);
            int v = (int)rndInt(1, n);
            if (u == v) continue;
            long long key = encode(u, v);
            if (used.count(key)) continue;
            used.insert(key);
            edges.emplace_back(u, v);
        }
    }

    // 为了生成合法的删除操作，需要维护当前存在的边集合
    // 我们先把所有边编号 0..m-1
    vector<int> alive(m, 1); // 1 表示当前存在
    vector<int> deletableIndices; // 当前仍可被删除的边的下标
    deletableIndices.reserve(m);
    for (int i = 0; i < m; ++i) deletableIndices.push_back(i);

    // 生成 q 个操作
    // 策略：随机选择操作类型，但保证删除操作不会超过 m 次且每次合法
    // 你可以根据需要调整三种操作的比例
    vector<tuple<int,int,long long>> ops; // (type, a, b)
    ops.reserve(q);

    int maxDelOps = m; // 最多能删 m 条边
    int usedDelOps = 0;

    for (int i = 0; i < q; ++i) {
        int type;
        // 如果还有可删边，就允许生成 1 操作，否则只生成 2/3
        if (usedDelOps < maxDelOps && !deletableIndices.empty()) {
            // 三种操作大致均匀随机
            type = (int)rndInt(1, 3);
        } else {
            // 不能再删边了
            type = (int)rndInt(2, 3);
        }

        if (type == 1) {
            // 删除一条当前存在的边
            int idxPos = (int)rndInt(0, (int)deletableIndices.size() - 1);
            int eidx = deletableIndices[idxPos];
            // 取出边 (a,b)
            int a = edges[eidx].first;
            int b = edges[eidx].second;

            // 标记删除
            alive[eidx] = 0;
            // 从 deletableIndices 中移除
            deletableIndices[idxPos] = deletableIndices.back();
            deletableIndices.pop_back();

            usedDelOps++;
            ops.emplace_back(1, a, b);
        } else if (type == 2) {
            // 增加城市发达程度
            int a = (int)rndInt(1, n);
            long long delta = rndInt(1, maxS); // 增加量
            ops.emplace_back(2, a, delta);
        } else {
            // 查询操作
            int a = (int)rndInt(1, n);
            long long b = rndInt(1, n); // 要求前 b 大
            ops.emplace_back(3, a, b);
        }
    }

    // 输出
    cout << n << " " << m << " " << q << "\n";
    for (int i = 1; i <= n; ++i) {
        cout << s[i] << (i == n ? '\n' : ' ');
    }
    for (int i = 0; i < m; ++i) {
        cout << edges[i].first << " " << edges[i].second << "\n";
    }
    for (auto &op : ops) {
        int t = get<0>(op);
        int a = get<1>(op);
        long long b = get<2>(op);
        cout << t << " " << a << " " << b << "\n";
    }

    return 0;
}
