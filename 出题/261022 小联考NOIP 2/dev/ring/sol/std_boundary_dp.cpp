#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 1'000'000'007;

// dp[u][s][a][b]: u is selected iff s; a and b are the states of the
// first and last leaves in u's DFS leaf sequence. All consecutive leaf
// edges inside that sequence have already been enforced.
static inline size_t id(int u, int s, int a, int b) {
    return (static_cast<size_t>(u) * 2 + s) * 4 + a * 2 + b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> parent(n + 1, 0), head(n + 1, 0), nxt(n + 1, 0);
    for (int u = 2; u <= n; ++u) cin >> parent[u];

    // Insert in descending vertex order so every child list is ascending.
    for (int u = n; u >= 2; --u) {
        nxt[u] = head[parent[u]];
        head[parent[u]] = u;
    }

    int leaf_count = 0;
    for (int u = 1; u <= n; ++u) leaf_count += (head[u] == 0);

    vector<int> dp(static_cast<size_t>(n + 1) * 8, 0);

    for (int u = n; u >= 1; --u) {
        if (head[u] == 0) {
            for (int s = 0; s <= 1; ++s) dp[id(u, s, s, s)] = 1;
            continue;
        }

        for (int s = 0; s <= 1; ++s) {
            int acc[2][2] = {{0, 0}, {0, 0}};
            bool first_child = true;

            for (int v = head[u]; v != 0; v = nxt[v]) {
                int child[2][2] = {{0, 0}, {0, 0}};
                for (int t = 0; t <= 1; ++t) {
                    if (s && t) continue;
                    for (int c = 0; c <= 1; ++c) {
                        for (int d = 0; d <= 1; ++d) {
                            int &value = child[c][d];
                            value += dp[id(v, t, c, d)];
                            if (value >= MOD) value -= MOD;
                        }
                    }
                }

                if (first_child) {
                    memcpy(acc, child, sizeof(acc));
                    first_child = false;
                    continue;
                }

                int merged[2][2] = {{0, 0}, {0, 0}};
                for (int a = 0; a <= 1; ++a) {
                    for (int b = 0; b <= 1; ++b) {
                        if (acc[a][b] == 0) continue;
                        for (int c = 0; c <= 1; ++c) {
                            if (b && c) continue;
                            for (int d = 0; d <= 1; ++d) {
                                if (child[c][d] == 0) continue;
                                int &value = merged[a][d];
                                value = (value + 1LL * acc[a][b] * child[c][d]) % MOD;
                            }
                        }
                    }
                }
                memcpy(acc, merged, sizeof(acc));
            }

            for (int a = 0; a <= 1; ++a) {
                for (int b = 0; b <= 1; ++b) dp[id(u, s, a, b)] = acc[a][b];
            }
        }
    }

    long long answer = 0;
    for (int s = 0; s <= 1; ++s) {
        for (int a = 0; a <= 1; ++a) {
            for (int b = 0; b <= 1; ++b) {
                // With one leaf there is no added edge. For two leaves the
                // closing edge duplicates the already checked path edge.
                if (leaf_count == 1 || !(a && b)) {
                    answer += dp[id(1, s, a, b)];
                    if (answer >= MOD) answer -= MOD;
                }
            }
        }
    }
    cout << answer << '\n';
    return 0;
}
