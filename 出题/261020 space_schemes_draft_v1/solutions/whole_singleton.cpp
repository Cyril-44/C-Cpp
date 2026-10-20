#include <bits/stdc++.h>
using namespace std;

// Independent verifier for subtask 6: every request is the whole segment or
// one leaf. No two-state segment-tree DP is used.
// Time O(n + q log n), memory O(n).
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;
    set<int> missing;
    for (int x = 1; x <= n; ++x) missing.insert(missing.end(), x);
    bool whole_required = false;

    for (int step = 0; step < q; ++step) {
        char operation;
        int l, r;
        cin >> operation >> l >> r;
        assert(operation == '+' || operation == '-');

        // For n=1 the whole interval and the singleton are the same request.
        if (l == r) {
            if (operation == '+') {
                const size_t erased = missing.erase(l);
                assert(erased == 1);
            } else {
                const bool inserted = missing.insert(l).second;
                assert(inserted);
            }
        } else {
            assert(l == 1 && r == n);
            assert(whole_required != (operation == '+'));
            whole_required = operation == '+';
        }

        const int active_leaves = n - int(missing.size());
        if (!whole_required || missing.empty()) {
            cout << active_leaves << " 1\n";
            continue;
        }

        // One extra node must contain every missing leaf. All such nodes
        // are the ancestors of the LCA of the extreme missing leaves.
        const int first_missing = *missing.begin();
        const int last_missing = *missing.rbegin();
        int a = 1, b = n, depth = 0;
        while (a < b) {
            const int mid = (a + b) / 2;
            if (last_missing <= mid) {
                b = mid;
            } else if (first_missing > mid) {
                a = mid + 1;
            } else {
                break;
            }
            ++depth;
        }
        cout << active_leaves + 1 << ' ' << depth + 1 << '\n';
    }
}
