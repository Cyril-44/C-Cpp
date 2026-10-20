#include <bits/stdc++.h>
using namespace std;

// Independent slow reference: iterative canonical decomposition, followed by
// a complete bottom-up recomputation after every operation.
// Time O(nq), memory O(n). Every traversal is iterative, including chain trees.
static constexpr int MOD = 998244353;
struct State { long long cost; int ways; };
struct Node {
    int left = 0, right = 0, first = 0, last = 0, count = 0;
    long long weight = 0;
    State required{0, 1}, full{0, 1};
};

State product(State a, State b) {
    return {a.cost + b.cost, int(1LL * a.ways * b.ways % MOD)};
}

State minimum(State a, State b) {
    if (a.cost < b.cost) return a;
    if (b.cost < a.cost) return b;
    int ways = a.ways + b.ways;
    if (ways >= MOD) ways -= MOD;
    return {a.cost, ways};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    const int vertices = 2 * n - 1;
    vector<Node> tree(vertices + 1);
    for (int v = 1; v <= vertices; ++v)
        cin >> tree[v].left >> tree[v].right >> tree[v].weight;

    vector<int> order, stack{1};
    order.reserve(vertices);
    int next_leaf = 0;
    while (!stack.empty()) {
        const int v = stack.back();
        stack.pop_back();
        order.push_back(v);
        if (tree[v].left == 0) {
            tree[v].first = tree[v].last = ++next_leaf;
        } else {
            stack.push_back(tree[v].right);
            stack.push_back(tree[v].left);
        }
    }
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        Node& node = tree[*it];
        if (node.left) {
            node.first = tree[node.left].first;
            node.last = tree[node.right].last;
        }
    }

    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        cin >> operation >> l >> r;
        const int delta = operation == '+' ? 1 : -1;
        stack.push_back(1);
        while (!stack.empty()) {
            const int v = stack.back();
            stack.pop_back();
            Node& node = tree[v];
            if (node.last < l || r < node.first) continue;
            if (l <= node.first && node.last <= r) {
                node.count += delta;
                assert(node.count >= 0);
            } else {
                stack.push_back(node.right);
                stack.push_back(node.left);
            }
        }
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            Node& node = tree[*it];
            if (node.left == 0) {
                node.full = {node.weight, 1};
                node.required = node.count ? node.full : State{0, 1};
                continue;
            }
            const State descendants = product(tree[node.left].required, tree[node.right].required);
            const State choose_root{descendants.cost + node.weight, descendants.ways};
            const State omit_root = product(tree[node.left].full, tree[node.right].full);
            node.full = minimum(choose_root, omit_root);
            node.required = node.count ? node.full : descendants;
        }
        cout << tree[1].required.cost << ' ' << tree[1].required.ways << '\n';
    }
}
