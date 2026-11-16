#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

struct Fenwick {
    int n;
    vector<int64> bit;
    explicit Fenwick(int n = 0) : n(n), bit(n + 1) {}
    void add(int p, int64 value) {
        for (; p <= n; p += p & -p) bit[p] += value;
    }
    int64 sumPrefix(int p) const {
        int64 result = 0;
        for (; p > 0; p -= p & -p) result += bit[p];
        return result;
    }
};

// Supports adding alpha * prefixDistance[i] + beta to every i in [l,r],
// and querying sums of the resulting point values on a range.
struct RangeLinearAddRangeSum {
    int n;
    const vector<int64>& prefixPosition;
    Fenwick alpha, beta, constant;

    RangeLinearAddRangeSum(int n, const vector<int64>& p)
        : n(n), prefixPosition(p), alpha(n), beta(n), constant(n) {}

    void event(int position, int64 da, int64 db) {
        if (position > n) return;
        alpha.add(position, da);
        beta.add(position, db);
        constant.add(position,
                     -da * prefixPosition[position - 1]
                     + db * (1LL - position));
    }

    void add(int left, int right, int64 a, int64 b) {
        if (left > right) return;
        event(left, a, b);
        event(right + 1, -a, -b);
    }

    int64 prefixSum(int right) const {
        if (right <= 0) return 0;
        return prefixPosition[right] * alpha.sumPrefix(right)
             + 1LL * right * beta.sumPrefix(right)
             + constant.sumPrefix(right);
    }

    int64 rangeSum(int left, int right) const {
        if (left > right) return 0;
        return prefixSum(right) - prefixSum(left - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int64> position(n + 1), prefixPosition(n + 1);
    for (int i = 2; i <= n; ++i) {
        int parent;
        int w;
        cin >> parent >> w;
        position[i] = position[i - 1] + w;
    }
    for (int i = 1; i <= n; ++i)
        prefixPosition[i] = prefixPosition[i - 1] + position[i];

    RangeLinearAddRangeSum values(n, prefixPosition);
    while (m--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            int y;
            cin >> y;
            int64 py = position[y];

            // x itself belongs to SSubTr(x).
            values.add(x, x, 0, llabs(position[x] - py));

            // On a chain, the remaining selected vertices form [x+2,n].
            int left = x + 2;
            int leftRight = min(n, y - 1);
            values.add(left, leftRight, -1, py);
            values.add(max(left, y), n, 1, -py);
        } else {
            int64 answer = values.rangeSum(x, x);
            answer += values.rangeSum(x + 2, n);
            cout << answer << '\n';
        }
    }
}
