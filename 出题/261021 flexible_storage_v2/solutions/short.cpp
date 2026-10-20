#include <bits/stdc++.h>
using namespace std;

// Subtask 9: every requested interval has length <= 2.
// A usable stored node is either a leaf or the parent of two leaf children.
// Those sibling pairs are disjoint components. A component has one or two
// optimal storage sets, so maintain total cost and the number of tied components.
// Time O(n+q), memory O(n). No canonical decomposition or general tree DP.
static constexpr int MOD = 998244353;
struct Component {
    int first, second; // second = 0 for an isolated leaf
    long long parent_weight;
    int pair_required = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    const int vertices = 2 * n - 1;
    vector<int> left(vertices + 1), right(vertices + 1);
    vector<long long> weight(vertices + 1);
    for (int v = 1; v <= vertices; ++v) cin >> left[v] >> right[v] >> weight[v];
    vector<int> leaf_order(vertices + 1, 0), stack{1};
    vector<long long> leaf_weight(n + 1);
    int leaf = 0;
    while (!stack.empty()) {
        const int v = stack.back();
        stack.pop_back();
        if (left[v] == 0) {
            leaf_order[v] = ++leaf;
            leaf_weight[leaf] = weight[v];
        } else {
            stack.push_back(right[v]);
            stack.push_back(left[v]);
        }
    }
    vector<Component> components;
    vector<int> component_id(n + 1, -1), leaf_required(n + 1, 0);
    for (int v = 1; v <= vertices; ++v) {
        if (left[v] && left[left[v]] == 0 && left[right[v]] == 0) {
            const int a = leaf_order[left[v]], b = leaf_order[right[v]];
            component_id[a] = component_id[b] = int(components.size());
            components.push_back({a, b, weight[v], 0});
        }
    }
    for (int i = 1; i <= n; ++i) {
        if (component_id[i] == -1) {
            component_id[i] = int(components.size());
            components.push_back({i, 0, 0, 0});
        }
    }
    auto evaluate = [&](int id) -> pair<long long, int> {
        const Component& component = components[id];
        long long mandatory = leaf_required[component.first] ? leaf_weight[component.first] : 0;
        if (component.second && leaf_required[component.second]) mandatory += leaf_weight[component.second];
        if (!component.pair_required) return {mandatory, 0};
        const long long use_parent = component.parent_weight + mandatory;
        const long long use_leaves = leaf_weight[component.first] + leaf_weight[component.second];
        return {min(use_parent, use_leaves), use_parent == use_leaves ? 1 : 0};
    };
    vector<int> power_of_two(n + 1, 1);
    for (int i = 1; i <= n; ++i) power_of_two[i] = int(2LL * power_of_two[i - 1] % MOD);
    long long total_cost = 0;
    int tied_components = 0;
    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        cin >> operation >> l >> r;
        if (r - l + 1 > 2) {
            cerr << "short.cpp requires interval length <= 2\n";
            return 2;
        }
        const int delta = operation == '+' ? 1 : -1;
        const int a = component_id[l], b = component_id[r];
        const auto old_a = evaluate(a);
        total_cost -= old_a.first;
        tied_components -= old_a.second;
        if (b != a) {
            const auto old_b = evaluate(b);
            total_cost -= old_b.first;
            tied_components -= old_b.second;
        }
        if (l != r && a == b) {
            components[a].pair_required += delta;
            assert(components[a].pair_required >= 0);
        } else {
            leaf_required[l] += delta;
            assert(leaf_required[l] >= 0);
            if (l != r) {
                leaf_required[r] += delta;
                assert(leaf_required[r] >= 0);
            }
        }
        const auto new_a = evaluate(a);
        total_cost += new_a.first;
        tied_components += new_a.second;
        if (b != a) {
            const auto new_b = evaluate(b);
            total_cost += new_b.first;
            tied_components += new_b.second;
        }
        cout << total_cost << ' ' << power_of_two[tied_components] << '\n';
    }
}
