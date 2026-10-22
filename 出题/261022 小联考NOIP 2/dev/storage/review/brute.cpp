#include <bits/stdc++.h>
using namespace std;

// Enumeration oracle for n <= 8. It does NOT use canonical decomposition or
// demand counters: every storage set P is tested directly against each active
// requested interval. Time O(2^(2n-1) * (n^3 + q)), memory O(2^(2n-1)).
static constexpr int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    if (n < 1 || n > 8) {
        cerr << "brute supports 1 <= n <= 8 only\n";
        return 2;
    }
    const int vertices = 2 * n - 1;
    vector<int> left(vertices + 1), right(vertices + 1);
    vector<long long> weight(vertices + 1);
    for (int v = 1; v <= vertices; ++v) cin >> left[v] >> right[v] >> weight[v];
    vector<unsigned> positions(vertices + 1, 0);
    vector<int> order, stack{1};
    int leaf = 0;
    while (!stack.empty()) {
        const int v = stack.back();
        stack.pop_back();
        order.push_back(v);
        if (left[v] == 0) positions[v] = 1u << leaf++;
        else {
            stack.push_back(right[v]);
            stack.push_back(left[v]);
        }
    }
    for (auto it = order.rbegin(); it != order.rend(); ++it)
        if (left[*it]) positions[*it] = positions[left[*it]] | positions[right[*it]];

    vector<vector<int>> id(n + 1, vector<int>(n + 1, -1));
    vector<unsigned> targets;
    for (int l = 1; l <= n; ++l) {
        unsigned mask = 0;
        for (int r = l; r <= n; ++r) {
            mask |= 1u << (r - 1);
            id[l][r] = int(targets.size());
            targets.push_back(mask);
        }
    }
    const int subsets = 1 << vertices;
    vector<uint64_t> representable(subsets, 0);
    vector<long long> cost(subsets, 0);
    for (int selected = 0; selected < subsets; ++selected) {
        for (int v = 1; v <= vertices; ++v)
            if (selected >> (v - 1) & 1) cost[selected] += weight[v];
        for (int target = 0; target < int(targets.size()); ++target) {
            unsigned covered = 0;
            for (int v = 1; v <= vertices; ++v) {
                if ((selected >> (v - 1) & 1) && !(positions[v] & ~targets[target]))
                    covered |= positions[v];
            }
            // Inclusion-maximal chosen nodes inside a target are disjoint.
            // Therefore union coverage is equivalent to a valid disjoint cover.
            if (covered == targets[target]) representable[selected] |= uint64_t(1) << target;
        }
    }

    uint64_t active = 0;
    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        cin >> operation >> l >> r;
        const uint64_t bit = uint64_t(1) << id[l][r];
        if (operation == '+') {
            assert(!(active & bit));
            active |= bit;
        } else {
            assert(active & bit);
            active &= ~bit;
        }
        long long best = LLONG_MAX;
        int ways = 0;
        for (int selected = 0; selected < subsets; ++selected) {
            if (cost[selected] > best || (representable[selected] & active) != active) continue;
            if (cost[selected] < best) {
                best = cost[selected];
                ways = 1;
            } else if (++ways == MOD) ways = 0;
        }
        cout << best << ' ' << ways << '\n';
    }
}
