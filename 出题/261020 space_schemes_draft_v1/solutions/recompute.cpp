#include <bits/stdc++.h>
using namespace std;

// Reference solver: change canonical-decomposition counters, then recompute
// every DP state in postorder. Time O(nq + q log n), memory O(n).
static constexpr int MOD = 998244353;

struct State {
    int cost;
    int ways;
};

State independent_union(State a, State b) {
    return {a.cost + b.cost, int(1LL * a.ways * b.ways % MOD)};
}

State best_of_disjoint_cases(State a, State b) {
    if (a.cost < b.cost) return a;
    if (b.cost < a.cost) return b;
    int ways = a.ways + b.ways;
    if (ways >= MOD) ways -= MOD;
    return {a.cost, ways};
}

struct Node {
    int l, r;
    int left = -1, right = -1;
    int required_count = 0;
    State required{0, 1}; // Satisfy all marked nodes in this subtree.
    State full{1, 1};     // Also require coverage of the whole node interval.
};

vector<Node> tree;

int build_tree(int l, int r) {
    const int v = int(tree.size());
    tree.push_back(Node{l, r});
    if (l != r) {
        const int mid = (l + r) / 2;
        const int left = build_tree(l, mid);
        const int right = build_tree(mid + 1, r);
        tree[v].left = left;
        tree[v].right = right;
    }
    return v;
}

void change_requirement(int v, int ql, int qr, int delta) {
    Node& node = tree[v];
    if (ql <= node.l && node.r <= qr) {
        node.required_count += delta;
        assert(node.required_count >= 0);
        return;
    }
    const int mid = (node.l + node.r) / 2;
    if (ql <= mid) change_requirement(node.left, ql, qr, delta);
    if (qr > mid) change_requirement(node.right, ql, qr, delta);
}

void recompute_all() {
    // Preorder allocation implies descendants always have larger indices.
    for (int v = int(tree.size()) - 1; v >= 0; --v) {
        Node& node = tree[v];
        if (node.l == node.r) {
            node.full = {1, 1};
            node.required = node.required_count ? node.full : State{0, 1};
            continue;
        }
        const Node& left = tree[node.left];
        const Node& right = tree[node.right];
        const State children = independent_union(left.required, right.required);
        const State store_this_node{children.cost + 1, children.ways};
        const State omit_this_node = independent_union(left.full, right.full);
        node.full = best_of_disjoint_cases(store_this_node, omit_this_node);
        node.required = node.required_count ? node.full : children;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;
    tree.reserve(2 * n - 1);
    build_tree(1, n);
    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        cin >> operation >> l >> r;
        change_requirement(0, l, r, operation == '+' ? 1 : -1);
        recompute_all();
        cout << tree[0].required.cost << ' ' << tree[0].required.ways << '\n';
    }
}
