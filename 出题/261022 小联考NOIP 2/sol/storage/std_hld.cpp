#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

// C++17. All traversals of the input tree are iterative.
static constexpr int MOD = 998244353;
static constexpr int NO_COLOR = 1000000000;
static constexpr long long INF = (1LL << 60);

struct Value {
    long long cost;
    int ways;
};
Value add(Value a, Value b) {
    if (a.cost != b.cost) return a.cost < b.cost ? a : b;
    int w = a.ways + b.ways;
    if (w >= MOD) w -= MOD;
    return {a.cost, w};
}
Value mul(Value a, Value b) {
    if (a.cost == INF || b.cost == INF) return {INF, 0};
    return {a.cost + b.cost, int(1LL * a.ways * b.ways % MOD)};
}
bool equalValue(Value a, Value b) {
    return a.cost == b.cost && a.ways == b.ways;
}
struct Matrix {
    // Separate arrays avoid padding each (cost, ways) pair.
    long long c[4];
    int w[4];
    Value get(int i) const { return {c[i], w[i]}; }
    void set(int i, Value x) { c[i] = x.cost; w[i] = x.ways; }
};
Matrix identity() {
    Matrix a;
    a.set(0, {0, 1}); a.set(1, {INF, 0});
    a.set(2, {INF, 0}); a.set(3, {0, 1});
    return a;
}
Matrix multiply(const Matrix &a, const Matrix &b) {
    Matrix z;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            int x = i * 2 + j;
            long long best = INF;
            int ways = 0;
            for (int k = 0; k < 2; ++k) {
                int ai = i * 2 + k, bi = k * 2 + j;
                if (a.c[ai] == INF || b.c[bi] == INF) continue;
                long long cost = a.c[ai] + b.c[bi];
                if (cost > best) continue;
                int count = int(1LL * a.w[ai] * b.w[bi] % MOD);
                if (cost < best) best = cost, ways = count;
                else {
                    ways += count;
                    if (ways >= MOD) ways -= MOD;
                }
            }
            z.c[x] = best; z.w[x] = ways;
        }
    }
    return z;
}

class Solver {
    struct Segment {
        Matrix product[4];
        int minimum[2], lazy[2];
    };
    int n, N, q, rootCount = 0;
    vector<int> lc, rc, parent, depth, leaves, heavy, light;
    vector<int> low, high, leafId, head, dfn, at, tail, heavyCount;
    vector<long long> weight;
    vector<Value> X, B;
    vector<Segment> seg;

    Matrix transition(int v, bool lightPositive) const {
        int s = light[v];
        Value c = lightPositive ? B[s] : X[s];
        Value wc = {weight[v] + c.cost, c.ways};
        Matrix m;
        if (heavyCount[v] == 0) {
            m.set(0, c); m.set(1, {INF, 0});
            m.set(2, wc); m.set(3, B[s]);
        } else {
            m.set(0, {INF, 0}); m.set(1, c);
            m.set(2, {INF, 0}); m.set(3, add(wc, B[s]));
        }
        return m;
    }
    void remakeLeaf(int p, int v) {
        if (!lc[v]) {
            for (auto &m : seg[p].product) m = identity();
            return;
        }
        int direction = light[v] == rc[v]; // 0: left, 1: right
        Matrix zero = transition(v, false), positive = transition(v, true);
        for (int mask = 0; mask < 4; ++mask)
            seg[p].product[mask] = (mask >> direction & 1) ? zero : positive;
    }
    void pull(int p, int left, int right) {
        Segment &s = seg[p];
        for (int d = 0; d < 2; ++d)
            s.minimum[d] = min(seg[left].minimum[d], seg[right].minimum[d]);
        int leftEligible = 0, rightEligible = 0;
        for (int d = 0; d < 2; ++d) {
            if (seg[left].minimum[d] == s.minimum[d]) leftEligible |= 1 << d;
            if (seg[right].minimum[d] == s.minimum[d]) rightEligible |= 1 << d;
        }
        for (int mask = 0; mask < 4; ++mask) {
            // Reuse equal products when a direction is absent from this segment.
            int effective = mask;
            for (int d = 0; d < 2; ++d)
                if (s.minimum[d] == NO_COLOR) effective &= ~(1 << d);
            if (effective != mask) s.product[mask] = s.product[effective];
            else s.product[mask] = multiply(seg[left].product[mask & leftEligible],
                                            seg[right].product[mask & rightEligible]);
        }
    }
    void tag(int p, int direction, int delta) {
        if (seg[p].minimum[direction] == NO_COLOR) return;
        seg[p].minimum[direction] += delta;
        seg[p].lazy[direction] += delta;
    }
    void push(int p, int left, int right) {
        for (int d = 0; d < 2; ++d) {
            int delta = seg[p].lazy[d];
            if (delta) {
                tag(left, d, delta); tag(right, d, delta);
                seg[p].lazy[d] = 0;
            }
        }
    }
    // Compact preorder layout: a segment with length k uses exactly 2*k-1 nodes.
    void build(int p, int l, int r) {
        seg[p].lazy[0] = seg[p].lazy[1] = 0;
        if (l == r) {
            int v = at[l];
            seg[p].minimum[0] = seg[p].minimum[1] = NO_COLOR;
            if (lc[v]) seg[p].minimum[light[v] == rc[v]] = 0;
            remakeLeaf(p, v);
            return;
        }
        int mid = (l + r) / 2, a = p + 1, b = p + 2 * (mid - l + 1);
        build(a, l, mid); build(b, mid + 1, r);
        pull(p, a, b);
    }
    void rangeAdd(int p, int l, int r, int ql, int qr, int direction, int delta) {
        if (seg[p].minimum[direction] == NO_COLOR) return;
        if (ql <= l && r <= qr) {
            tag(p, direction, delta);
            return;
        }
        int mid = (l + r) / 2, a = p + 1, b = p + 2 * (mid - l + 1);
        push(p, a, b);
        if (ql <= mid) rangeAdd(a, l, mid, ql, qr, direction, delta);
        if (qr > mid) rangeAdd(b, mid + 1, r, ql, qr, direction, delta);
        pull(p, a, b);
    }
    void rangeAdd(int l, int r, int direction, int delta) {
        if (l <= r) rangeAdd(1, 1, N, l, r, direction, delta);
    }
    void pointRemake(int p, int l, int r, int pos) {
        if (l == r) {
            remakeLeaf(p, at[l]);
            return;
        }
        int mid = (l + r) / 2, a = p + 1, b = p + 2 * (mid - l + 1);
        push(p, a, b);
        if (pos <= mid) pointRemake(a, l, mid, pos);
        else pointRemake(b, mid + 1, r, pos);
        pull(p, a, b);
    }
    Matrix query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            int mask = (seg[p].minimum[0] == 0) | ((seg[p].minimum[1] == 0) << 1);
            return seg[p].product[mask];
        }
        int mid = (l + r) / 2, a = p + 1, b = p + 2 * (mid - l + 1);
        push(p, a, b);
        if (qr <= mid) return query(a, l, mid, ql, qr);
        if (ql > mid) return query(b, mid + 1, r, ql, qr);
        return multiply(query(a, l, mid, ql, qr),
                        query(b, mid + 1, r, ql, qr));
    }
    bool contained(int v, int l, int r) const {
        return low[v] >= l && high[v] <= r;
    }
    int maximal(int v, int l, int r) const {
        while (true) {
            int h = head[v];
            if (contained(h, l, r)) {
                int p = parent[h];
                if (!p || !contained(p, l, r)) return h;
                v = p;
            } else {
                int a = dfn[h], b = dfn[v];
                while (a < b) {
                    int mid = (a + b) / 2;
                    if (contained(at[mid], l, r)) b = mid;
                    else a = mid + 1;
                }
                return at[a];
            }
        }
    }
    int lca(int a, int b) const {
        while (head[a] != head[b]) {
            if (depth[head[a]] < depth[head[b]]) swap(a, b);
            a = parent[head[a]];
        }
        return depth[a] < depth[b] ? a : b;
    }
    void addHeavy(int p, int delta) {
        bool before = heavyCount[p] != 0;
        heavyCount[p] += delta;
        if (before != (heavyCount[p] != 0))
            pointRemake(1, 1, N, dfn[p]);
    }
    void addNode(int v, int delta) {
        int p = parent[v];
        if (!p) rootCount += delta;
        else if (heavy[p] == v) addHeavy(p, delta);
        else rangeAdd(dfn[p], dfn[p], light[p] == rc[p], delta);
    }
    void boundary(int v, int ancestor, int direction, int delta) {
        // Enumerate path edges except the one whose parent is ancestor.
        while (depth[v] > depth[ancestor] + 1) {
            int h = head[v];
            if (h == head[ancestor]) {
                rangeAdd(dfn[ancestor] + 1, dfn[v] - 1, direction, delta);
                break;
            }
            rangeAdd(dfn[h], dfn[v] - 1, direction, delta);
            int p = parent[h];
            if (depth[h] > depth[ancestor] + 1 &&
                (heavy[p] == rc[p]) == bool(direction))
                addHeavy(p, delta);
            v = p;
        }
    }
    void collect(int v, vector<int> &chains) const {
        while (v) {
            chains.push_back(head[v]);
            v = parent[head[v]];
        }
    }
    void finish(vector<int> &chains) {
        sort(chains.begin(), chains.end(), [&](int a, int b) {
            if (depth[a] != depth[b]) return depth[a] > depth[b];
            return a < b;
        });
        chains.erase(unique(chains.begin(), chains.end()), chains.end());
        for (int h : chains) {
            Matrix m = query(1, 1, N, dfn[h], dfn[tail[h]]);
            Value endX = {0, 1}, endB = {weight[tail[h]], 1};
            Value nx = add(mul(m.get(0), endX), mul(m.get(1), endB));
            Value nb = add(mul(m.get(2), endX), mul(m.get(3), endB));
            if (equalValue(nx, X[h]) && equalValue(nb, B[h])) continue;
            X[h] = nx; B[h] = nb;
            int p = parent[h];
            if (p) pointRemake(1, 1, N, dfn[p]);
        }
    }

public:
    void run() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        if (!(cin >> n >> q)) return;
        N = 2 * n - 1;
        lc.resize(N + 1); rc.resize(N + 1); parent.resize(N + 1);
        depth.resize(N + 1); leaves.resize(N + 1);
        heavy.resize(N + 1); light.resize(N + 1);
        low.resize(N + 1); high.resize(N + 1); leafId.resize(n + 1);
        head.resize(N + 1); dfn.resize(N + 1); at.resize(N + 1);
        tail.resize(N + 1); heavyCount.resize(N + 1);
        weight.resize(N + 1); X.resize(N + 1); B.resize(N + 1);
        for (int v = 1; v <= N; ++v) {
            cin >> lc[v] >> rc[v] >> weight[v];
            if (lc[v]) parent[lc[v]] = parent[rc[v]] = v;
        }
        vector<int> order, stack = {1};
        order.reserve(N);
        int leafNumber = 0;
        while (!stack.empty()) {
            int v = stack.back(); stack.pop_back();
            order.push_back(v);
            if (!lc[v]) {
                low[v] = high[v] = ++leafNumber;
                leafId[leafNumber] = v;
            } else {
                depth[lc[v]] = depth[rc[v]] = depth[v] + 1;
                stack.push_back(rc[v]); stack.push_back(lc[v]);
            }
        }
        for (int i = N - 1; i >= 0; --i) {
            int v = order[i];
            X[v] = {0, 1};
            if (!lc[v]) {
                leaves[v] = 1; B[v] = {weight[v], 1};
            } else {
                low[v] = low[lc[v]]; high[v] = high[rc[v]];
                leaves[v] = leaves[lc[v]] + leaves[rc[v]];
                // Ties choose the right child, also used by subtask 7.
                heavy[v] = leaves[rc[v]] >= leaves[lc[v]] ? rc[v] : lc[v];
                light[v] = heavy[v] == lc[v] ? rc[v] : lc[v];
                B[v] = add({weight[v], 1}, mul(B[lc[v]], B[rc[v]]));
            }
        }
        stack = {1};
        int timer = 0;
        while (!stack.empty()) {
            int h = stack.back(); stack.pop_back();
            int v = h;
            while (v) {
                head[v] = h; dfn[v] = ++timer; at[timer] = v;
                tail[h] = v;
                if (light[v]) stack.push_back(light[v]);
                v = heavy[v];
            }
        }
        seg.resize(2 * N + 2);
        build(1, 1, N);
        vector<int> chains;
        chains.reserve(64);
        for (int i = 0; i < q; ++i) {
            char op; int l, r;
            cin >> op >> l >> r;
            int delta = op == '+' ? 1 : -1;
            int a = maximal(leafId[l], l, r), b = maximal(leafId[r], l, r);
            chains.clear();
            collect(parent[a], chains);
            if (a != b) collect(parent[b], chains);
            addNode(a, delta);
            if (a != b) {
                addNode(b, delta);
                int z = lca(a, b);
                boundary(a, z, 1, delta);
                boundary(b, z, 0, delta);
            }
            finish(chains);
            Value ans = rootCount ? B[1] : X[1];
            cout << ans.cost << ' ' << ans.ways << '\n';
        }
    }
};
int main() {
    Solver solver;
    solver.run();
    return 0;
}
