#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// 出错时先给选手输出 -1(题面要求选手读到 -1 就退出),再以 WA 结束
#define FAIL(...)                \
    do {                         \
        cout << -1 << endl;      \
        quitf(_wa, __VA_ARGS__); \
    } while (0)

int main(int argc, char *argv[]) {
    registerInteraction(argc, argv);

    // 输入文件格式(与 hack 格式相同):
    // t
    // n
    // n-1 行边 u v
    // ...
    int t = inf.readInt();
    cout << t << endl;

    int maxQueries = 0;

    for (int tc = 1; tc <= t; tc++) {
        int n = inf.readInt();
        vector<vector<int>> g(n + 1);
        set<pair<int, int>> truth;
        for (int i = 0; i < n - 1; i++) {
            int u = inf.readInt(1, n);
            int v = inf.readInt(1, n);
            g[u].push_back(v);
            g[v].push_back(u);
            truth.insert({min(u, v), max(u, v)});
        }

        cout << n << endl;

        int queries = 0;
        const int limit = 2 * n - 2;

        while (true) {
            string op = ouf.readToken();

            if (op == "?") {
                if (++queries > limit)
                    FAIL("test %d: too many queries (limit is %d)", tc, limit);

                int x = ouf.readInt();
                if (x < 1 || x > n)
                    FAIL("test %d: x = %d is out of range [1, %d]", tc, x, n);

                vector<int> p(n + 1);
                vector<char> used(n + 1, 0);
                for (int i = 1; i <= n; i++) {
                    p[i] = ouf.readInt();
                    if (p[i] < 1 || p[i] > n)
                        FAIL("test %d: p_%d = %d is out of range [1, %d]", tc, i, p[i], n);
                    if (used[p[i]])
                        FAIL("test %d: p is not a permutation, value %d appears twice", tc, p[i]);
                    used[p[i]] = 1;
                }

                // 从 x 出发 BFS,mx[v] = path(v, x) 上 p 的最大值
                vector<int> mx(n + 1, 0), q;
                vector<char> vis(n + 1, 0);
                q.reserve(n);
                q.push_back(x);
                vis[x] = 1;
                mx[x] = p[x];
                for (size_t h = 0; h < q.size(); h++) {
                    int u = q[h];
                    for (int v : g[u]) {
                        if (!vis[v]) {
                            vis[v] = 1;
                            mx[v] = max(mx[u], p[v]);
                            q.push_back(v);
                        }
                    }
                }

                long long ans = 0;
                for (int i = 1; i <= n; i++) ans += mx[i];
                cout << ans << endl;
            } else if (op == "!") {
                set<pair<int, int>> got;
                for (int i = 0; i < n - 1; i++) {
                    int u = ouf.readInt();
                    int v = ouf.readInt();
                    if (u < 1 || u > n || v < 1 || v > n)
                        FAIL("test %d: edge (%d, %d) has a vertex out of range", tc, u, v);
                    if (u == v)
                        FAIL("test %d: edge (%d, %d) is a self-loop", tc, u, v);
                    got.insert({min(u, v), max(u, v)});
                }
                if (got != truth)
                    FAIL("test %d: the reported tree is wrong", tc);
                break;
            } else {
                FAIL("test %d: unknown operation '%s', expected '?' or '!'", tc, op.c_str());
            }
        }

        maxQueries = max(maxQueries, queries);
    }

    quitf(_ok, "%d test case(s) passed, max queries in a single test: %d", t, maxQueries);
}