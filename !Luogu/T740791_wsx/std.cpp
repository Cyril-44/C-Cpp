#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> a(n+1), pos(n); // a: 1-indexed positions -> value; pos[value] = position (1-indexed)
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        pos[a[i]] = i;
    }

    // iterative segment tree for min and max over indices 0..n-1 (values)
    int size = 1;
    while (size < n) size <<= 1;
    vector<int> segMin(2*size, INT_MAX), segMax(2*size, INT_MIN);

    // build leaves
    for (int v = 0; v < n; ++v) {
        segMin[size + v] = pos[v];
        segMax[size + v] = pos[v];
    }
    for (int i = size - 1; i >= 1; --i) {
        segMin[i] = min(segMin[i<<1], segMin[i<<1|1]);
        segMax[i] = max(segMax[i<<1], segMax[i<<1|1]);
    }

    auto update_value_pos = [&](int value, int newpos) {
        int idx = size + value;
        segMin[idx] = segMax[idx] = newpos;
        idx >>= 1;
        while (idx) {
            segMin[idx] = min(segMin[idx<<1], segMin[idx<<1|1]);
            segMax[idx] = max(segMax[idx<<1], segMax[idx<<1|1]);
            idx >>= 1;
        }
    };

    // query on [0, k-1] (inclusive) -> in iterative segtree use [l, r) style
    auto query_min_max_prefix = [&](int k) -> pair<int,int> {
        if (k <= 0) return {INT_MAX, INT_MIN}; // empty set
        int l = size + 0, r = size + k; // [l, r)
        int curMin = INT_MAX, curMax = INT_MIN;
        while (l < r) {
            if (l & 1) {
                curMin = min(curMin, segMin[l]);
                curMax = max(curMax, segMax[l]);
                ++l;
            }
            if (r & 1) {
                --r;
                curMin = min(curMin, segMin[r]);
                curMax = max(curMax, segMax[r]);
            }
            l >>= 1; r >>= 1;
        }
        return {curMin, curMax};
    };

    auto A_of = [&](int k) -> ll {
        if (k == 0) {
            return (ll)n * (n + 1) / 2;
        }
        if (k > n) return 0;
        auto pr = query_min_max_prefix(k); // min pos among 0..k-1, max pos among 0..k-1
        int L = pr.first, R = pr.second;
        if (L == INT_MAX || R == INT_MIN) return 0; // defensive
        return (ll)L * (ll)(n - R + 1);
    };

    for (int qi = 0; qi < m; ++qi) {
        int type, x, y;
        cin >> type >> x >> y;
        if (type == 1) {
            // swap values at positions x and y
            if (x == y) continue;
            int va = a[x], vb = a[y];
            // swap values in array
            swap(a[x], a[y]);
            // update positions
            pos[va] = y;
            pos[vb] = x;
            // update leaves in segtree
            update_value_pos(va, pos[va]);
            update_value_pos(vb, pos[vb]);
        } else if (type == 2) {
            int Lq = x, Rq = y;
            if (Lq > Rq) swap(Lq, Rq);
            // answer = A_x - A_{y+1}
            ll ans = A_of(Lq) - A_of(Rq + 1);
            cout << ans << '\n';
        }
    }
    return 0;
}