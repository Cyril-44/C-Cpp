#include <bits/stdc++.h>
using namespace std;

// 检查两条边 (a,b) 和 (c,d) 是否交叉（按题面定义）
bool is_cross(int a, int b, int c, int d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return (min(a, b) < min(c, d) && min(c, d) < max(a, b) && max(a, b) < max(c, d));
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 可以通过命令行参数控制规模：n, q, w_type
    // w_type = 0: 一般权值 [1, 10]
    // w_type = 1: 全部权值 = 1（模拟性质 B）
    int n = 10;          // 默认小 n
    int q = 20;          // 默认小 q
    int w_type = 0;      // 默认一般权值

    if (argc >= 2) n = atoi(argv[1]);
    if (argc >= 3) q = atoi(argv[2]);
    if (argc >= 4) w_type = atoi(argv[3]);

    if (n < 2) n = 2;
    if (q < 1) q = 1;

    // 额外边数量：最多 n-2，保证 m <= 2n-3
    int max_extra = max(0, n - 2);
    int extra = max_extra ? (rand() % (max_extra + 1)) : 0;
    int m = (n - 1) + extra;

    // 随机种子
    mt19937_64 rng(
        chrono::steady_clock::now().time_since_epoch().count()
    );

    // 存边： (u, v, w)
    vector<tuple<int,int,long long>> edges;

    // 先加链边：保证 u_i = i, v_i = i+1
    for (int i = 1; i < n; ++i) {
        long long w;
        if (w_type == 1) w = 1;
        else w = (rng() % 10) + 1; // 小权值方便手算
        edges.emplace_back(i, i + 1, w);
    }

    // 记录已有非链边，用于检查交叉和重边
    vector<pair<int,int>> extra_edges;

    auto has_edge = [&](int u, int v) {
        if (u > v) swap(u, v);
        // 链边
        if (v == u + 1) return true;
        // 非链边
        for (auto &e : extra_edges) {
            if (e.first == u && e.second == v) return true;
        }
        return false;
    };

    // 尝试加入若干非交叉额外边
    int attempts = 0;
    while ((int)extra_edges.size() < extra && attempts < 10000) {
        ++attempts;
        int u = uniform_int_distribution<int>(1, n)(rng);
        int v = uniform_int_distribution<int>(1, n)(rng);
        if (u == v) continue;
        if (u > v) swap(u, v);

        // 不要链边
        if (v == u + 1) continue;

        // 不要重边
        if (has_edge(u, v)) continue;

        // 检查与已有非链边是否交叉
        bool ok = true;
        for (auto &e : extra_edges) {
            int a = e.first, b = e.second;
            if (is_cross(a, b, u, v)) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        // 可以加入
        long long w;
        if (w_type == 1) w = 1;
        else w = (rng() % 10) + 1;
        edges.emplace_back(u, v, w);
        extra_edges.emplace_back(u, v);
    }

    // 最终 m 可能比预期少一点（如果实在加不进去），但一定满足 m >= n-1
    m = (int)edges.size();

    // 输出到 stdout（你可以重定向到 chain.in）
    cout << n << " " << m << " " << q << "\n";
    for (auto &e : edges) {
        int u, v;
        long long w;
        tie(u, v, w) = e;
        cout << u << " " << v << " " << w << "\n";
    }

    // 生成 q 个询问，随机两点，x != y
    for (int i = 0; i < q; ++i) {
        int x = uniform_int_distribution<int>(1, n)(rng);
        int y = uniform_int_distribution<int>(1, n)(rng);
        if (x == y) {
            // 简单修正：往右或往左挪一个
            if (x < n) y = x + 1;
            else y = x - 1;
        }
        cout << x << " " << y << "\n";
    }

    return 0;
}
