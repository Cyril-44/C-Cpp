#include <bits/stdc++.h>
using namespace std;

// Independent oracle: enumerate every storage set P, without using the DP.
// Intended for n <= 8 (at most 15 tree nodes and 36 possible requests).
// Time: O(2^(2n-1) * n^3 + q * 2^(2n-1)); memory: O(2^(2n-1)).
static constexpr int MOD = 998244353;

struct TreeNode {
    unsigned positions;
};

void build_tree(int l, int r, vector<TreeNode>& nodes) {
    unsigned positions = 0;
    for (int x = l; x <= r; ++x) positions |= 1u << (x - 1);
    nodes.push_back({positions});
    if (l != r) {
        const int mid = (l + r) / 2;
        build_tree(l, mid, nodes);
        build_tree(mid + 1, r, nodes);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;
    if (n < 1 || n > 8) {
        cerr << "brute.cpp supports 1 <= n <= 8 only.\n";
        return 2;
    }

    vector<TreeNode> nodes;
    build_tree(1, n, nodes);

    vector<vector<int>> interval_id(n + 1, vector<int>(n + 1, -1));
    vector<unsigned> interval_positions;
    for (int l = 1; l <= n; ++l) {
        unsigned positions = 0;
        for (int r = l; r <= n; ++r) {
            positions |= 1u << (r - 1);
            interval_id[l][r] = int(interval_positions.size());
            interval_positions.push_back(positions);
        }
    }

    const int number_of_sets = 1 << int(nodes.size());
    vector<uint64_t> representable(number_of_sets, 0);
    vector<unsigned char> size(number_of_sets, 0);
    for (int p = 0; p < number_of_sets; ++p) {
        size[p] = static_cast<unsigned char>(__builtin_popcount(unsigned(p)));
        for (int id = 0; id < int(interval_positions.size()); ++id) {
            const unsigned target = interval_positions[id];
            unsigned covered = 0;
            for (int j = 0; j < int(nodes.size()); ++j) {
                if ((p >> j & 1) && !(nodes[j].positions & ~target)) {
                    covered |= nodes[j].positions;
                }
            }
            // Tree intervals form a laminar family. If their union covers the
            // target, its inclusion-maximal members are a disjoint cover.
            if (covered == target) representable[p] |= uint64_t(1) << id;
        }
    }

    uint64_t active = 0;
    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        if (!(cin >> operation >> l >> r) || l < 1 || l > r || r > n) {
            cerr << "Invalid operation at step " << step + 1 << ".\n";
            return 2;
        }
        const uint64_t bit = uint64_t(1) << interval_id[l][r];
        if (operation == '+' && !(active & bit)) {
            active |= bit;
        } else if (operation == '-' && (active & bit)) {
            active &= ~bit;
        } else {
            cerr << "Illegal insertion or deletion at step " << step + 1 << ".\n";
            return 2;
        }

        int best = int(nodes.size()) + 1;
        int ways = 0;
        for (int p = 0; p < number_of_sets; ++p) {
            if (size[p] > best || (representable[p] & active) != active) continue;
            if (size[p] < best) {
                best = size[p];
                ways = 1;
            } else {
                if (++ways == MOD) ways = 0;
            }
        }
        cout << best << ' ' << ways << '\n';
    }
}
