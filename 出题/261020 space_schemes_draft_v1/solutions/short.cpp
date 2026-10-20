#include <bits/stdc++.h>
using namespace std;

// Independent verifier for subtask 7: every request has length at most two.
// The tree is partitioned into disjoint two-leaf components and isolated
// leaves. No general-tree DP is used. Time O(n+q), memory O(n).
static constexpr int MOD = 998244353;

struct Component {
    int l, r;
    int whole = 0;
    int first = 0;
    int second = 0;
};

vector<Component> components;
vector<int> component_of;
int total_cost = 0;
int exponent = 0;

void build_components(int l, int r) {
    if (r - l <= 1) {
        const int id = int(components.size());
        components.push_back(Component{l, r});
        for (int x = l; x <= r; ++x) component_of[x] = id;
        return;
    }
    const int mid = (l + r) / 2;
    build_components(l, mid);
    build_components(mid + 1, r);
}

pair<int, int> contribution(const Component& c) {
    if (c.l == c.r) return {int(c.first > 0), 0};
    const int leaves = int(c.first > 0) + int(c.second > 0);
    if (!c.whole) return {leaves, 0};
    if (!leaves) return {1, 0};
    return {2, int(leaves == 1)};
}

void change_counter(int id, int& counter, int delta) {
    const auto before = contribution(components[id]);
    total_cost -= before.first;
    exponent -= before.second;
    counter += delta;
    assert(counter >= 0);
    const auto after = contribution(components[id]);
    total_cost += after.first;
    exponent += after.second;
}

void change_leaf(int x, int delta) {
    const int id = component_of[x];
    Component& c = components[id];
    if (x == c.l) change_counter(id, c.first, delta);
    else change_counter(id, c.second, delta);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;
    component_of.resize(n + 1);
    components.reserve(n);
    build_components(1, n);

    vector<int> powers(n + 1, 1);
    for (int i = 1; i <= n; ++i) powers[i] = int(2LL * powers[i - 1] % MOD);

    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        cin >> operation >> l >> r;
        assert(operation == '+' || operation == '-');
        assert(1 <= l && l <= r && r <= n && r - l <= 1);
        const int delta = operation == '+' ? 1 : -1;
        if (l == r) {
            change_leaf(l, delta);
        } else if (component_of[l] == component_of[r]) {
            const int id = component_of[l];
            change_counter(id, components[id].whole, delta);
        } else {
            // This length-two interval crosses a tree split, so its
            // canonical decomposition consists of two singleton leaves.
            change_leaf(l, delta);
            change_leaf(r, delta);
        }
        assert(0 <= exponent && exponent <= n);
        cout << total_cost << ' ' << powers[exponent] << '\n';
    }
}
