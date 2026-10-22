#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> parent(n + 1, 0);
    vector<vector<int>> children(n + 1);
    for (int u = 2; u <= n; ++u) {
        cin >> parent[u];
        children[parent[u]].push_back(u);
    }

    vector<int> leaves;
    vector<int> stack = {1};
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if (children[u].empty()) {
            leaves.push_back(u);
        } else {
            for (auto it = children[u].rbegin(); it != children[u].rend(); ++it)
                stack.push_back(*it);
        }
    }

    long long answer = 0;
    const unsigned int limit = 1U << n;
    for (unsigned int mask = 0; mask < limit; ++mask) {
        bool valid = true;
        for (int u = 2; u <= n && valid; ++u) {
            if ((mask >> (u - 1) & 1U) && (mask >> (parent[u] - 1) & 1U)) valid = false;
        }
        if (!valid) continue;

        int k = static_cast<int>(leaves.size());
        if (k >= 2) {
            for (int i = 0; i < k; ++i) {
                int j = (i + 1) % k;
                if ((mask >> (leaves[i] - 1) & 1U) && (mask >> (leaves[j] - 1) & 1U)) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) ++answer;
    }

    cout << answer % MOD << '\n';
    return 0;
}
