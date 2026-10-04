#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, P;
    cin >> n >> m >> P;

    if (m < n) {
        cout << 0 << '\n';
        return 0;
    }

    int H = m - 2 * n;
    if (H <= 0) {
        cout << 1 << '\n';
        return 0;
    }

    auto add = [&](int &x, int y) {
        x += y;
        if (x >= P) x -= P;
    };

    vector<vector<int>> C(202, vector<int>(202));
    C[0][0] = 1;
    for (int i = 1; i <= 201; ++i) {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            C[i][j] = C[i - 1][j - 1];
            add(C[i][j], C[i - 1][j]);
        }
    }

    auto choose = [&](int a, int b) -> int {
        if (a < 0 || b < 0 || b > a) return 0;
        return C[a][b];
    };

    // 小元素为 [1, ..., k]，其余元素可以任意选择。
    int ans = 0;
    for (int k = 0; k <= n; ++k)
        add(ans, choose(H, n - k));

    int R = (n - 1) / 2;
    if (R == 0) {
        cout << ans << '\n';
        return 0;
    }

    int Q = R - 1, D = 2 * R;
    int sd = (H + 1) * (D + 1);

    // dist[N][s]：N 个上升步，上升步到达高度之和为 s。
    vector<vector<int>> dist(Q + 1, vector<int>(H + 1));
    vector<int> paths((Q + 1) * (Q + 1) * (H + 1));

    auto pi = [&](int u, int v, int s) {
        return (u * (Q + 1) + v) * (H + 1) + s;
    };

    paths[pi(0, 0, 0)] = 1;
    for (int u = 0; u <= Q; ++u) {
        for (int v = 0; v <= u; ++v) {
            int h = u - v + 1;
            for (int s = 0; s <= H; ++s) {
                int z = paths[pi(u, v, s)];
                if (!z) continue;

                if (u == v) add(dist[u][s], z);
                if (u < Q && s + h <= H)
                    add(paths[pi(u + 1, v, s + h)], z);
                if (v < u)
                    add(paths[pi(u, v + 1, s)], z);
            }
        }
    }

    // member[N][s][d]：跨度为 s 且包含坐标 d 的规范块数量。
    vector<int> member((Q + 1) * sd);

    auto mi = [&](int N, int s, int d) {
        return N * sd + s * (D + 1) + d;
    };

    // 每个规范块都包含末点 s。
    for (int N = 0; N <= Q; ++N)
        for (int s = 0; s <= min(H, D); ++s)
            add(member[mi(N, s, s)], dist[N][s]);

    int count = (Q + 1) * (Q + 1) * sd;
    vector<int> before(count), after(count);

    auto wi = [&](int u, int v, int s, int d) {
        return (u * (Q + 1) + v) * sd + s * (D + 1) + d;
    };

    // 标记一个到达高度 l+1 的上升步。
    for (int l = 0; l < Q; ++l) {
        fill(before.begin(), before.end(), 0);
        fill(after.begin(), after.end(), 0);
        before[wi(0, 0, 0, 0)] = 1;

        for (int u = 0; u <= Q; ++u) {
            for (int v = 0; v <= u; ++v) {
                int h = u - v + 1;
                for (int s = 0; s <= H; ++s) {
                    for (int d = 0; d <= D; ++d) {
                        int pos = wi(u, v, s, d);
                        int a = before[pos], b = after[pos];
                        if (!a && !b) continue;

                        if (u == v)
                            add(member[mi(u, s, d)], b);

                        if (v < u) {
                            int nx = wi(u, v + 1, s, d);
                            add(before[nx], a);
                            add(after[nx], b);
                        }

                        if (u == Q || s + h > H) continue;

                        int d0 = min(h, l) + (h > l);
                        int d1 = min(h, l);

                        if (d + d0 <= D)
                            add(before[wi(u + 1, v, s + h, d + d0)], a);

                        if (d + d1 <= D)
                            add(after[wi(u + 1, v, s + h, d + d1)], b);

                        if (h == l + 1 && d + l <= D)
                            add(after[wi(u + 1, v, s + h, d + l)], a);
                    }
                }
            }
        }
    }

    // 小元素为 [1, ..., k-1, 2k+1]。
    for (int k = 1; k <= R; ++k) {
        int N = k - 1;
        int T = n - 2 * k - 1;

        for (int s = 0; s <= H; ++s) {
            for (int g = 1; g <= 2 * k + 1; ++g) {
                int d = 2 * k + 1 - g;
                int ways = 0, mult = 0;

                if (d <= s) {
                    ways = member[mi(N, s, d)];
                    mult = choose(H - s - g, T + 1);
                } else if (H >= 2 * k + 2 && T > 0) {
                    ways = dist[N][s];
                    mult = choose(H - s - g - 1, T)
                         - choose(d - s - 1, T);
                    if (mult < 0) mult += P;
                }

                add(ans, int(1LL * ways * mult % P));
            }
        }
    }

    cout << ans << '\n';
    return 0;
}