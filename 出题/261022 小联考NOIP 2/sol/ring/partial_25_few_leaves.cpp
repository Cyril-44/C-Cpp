#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> parent(n + 1, 0), head(n + 1, 0), nxt(n + 1, 0);
    for (int u = 2; u <= n; ++u) {
        cin >> parent[u];
        nxt[u] = head[parent[u]];
        head[parent[u]] = u;
    }

    vector<int> leaves;
    vector<int> stack = {1};
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if (head[u] == 0) {
            leaves.push_back(u);
        } else {
            // head/nxt list children in descending order; this makes the
            // smallest child the next one popped from the stack.
            for (int v = head[u]; v != 0; v = nxt[v]) stack.push_back(v);
        }
    }

    const int k = static_cast<int>(leaves.size());
    if (k > 8) return 2;
    vector<int> leaf_index(n + 1, -1);
    for (int i = 0; i < k; ++i) leaf_index[leaves[i]] = i;

    long long answer = 0;
    const int masks = 1 << k;
    vector<int> dp0(n + 1), dp1(n + 1);

    for (int mask = 0; mask < masks; ++mask) {
        bool valid = true;
        if (k >= 2) {
            for (int i = 0; i < k; ++i) {
                int j = (i + 1) % k;
                if ((mask >> i & 1) && (mask >> j & 1)) {
                    valid = false;
                    break;
                }
            }
        }
        if (!valid) continue;

        for (int u = n; u >= 1; --u) {
            if (head[u] == 0) {
                bool selected = (mask >> leaf_index[u]) & 1;
                dp0[u] = selected ? 0 : 1;
                dp1[u] = selected ? 1 : 0;
                continue;
            }

            long long ways0 = 1, ways1 = 1;
            for (int v = head[u]; v != 0; v = nxt[v]) {
                ways0 = ways0 * (dp0[v] + dp1[v]) % MOD;
                ways1 = ways1 * dp0[v] % MOD;
            }
            dp0[u] = static_cast<int>(ways0);
            dp1[u] = static_cast<int>(ways1);
        }
        answer += dp0[1] + dp1[1];
        answer %= MOD;
    }

    cout << answer << '\n';
    return 0;
}
