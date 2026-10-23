#include <bits/stdc++.h>
using namespace std;

constexpr int P = 998244353;

int add(int a, int b) { return a + b >= P ? a + b - P : a + b; }
int sub(int a, int b) { return a >= b ? a - b : a - b + P; }
int mul(int a, int b) { return int(1LL * a * b % P); }

// 段内下标从 0 开始：sum = Σd[i]，moment = Σi*d[i]。
struct Info {
    int len = 0, sum = 0, moment = 0;
};

Info operator+(Info a, Info b) {
    return {a.len + b.len, add(a.sum, b.sum),
            add(add(a.moment, b.moment), mul(a.len, b.sum))};
}

struct WBLT {
    struct Node {
        int ls = 0, rs = 0, w = 0, refcnt = 0;
        int sum = 0, moment = 0, tag = 1;
    };

    vector<Node> tr;
    vector<int> bin;

    explicit WBLT(int n) : tr(1) {
        tr.reserve(2 * n + 128);
    }

    int size(int u) const { return tr[u].w; }
    Info info(int u) const { return {tr[u].w, tr[u].sum, tr[u].moment}; }

    int alloc() {
        int u;
        if (bin.empty()) {
            u = int(tr.size());
            tr.push_back({});
        } else {
            u = bin.back();
            bin.pop_back();
            tr[u] = {};
        }
        tr[u].refcnt = 1;
        return u;
    }

    void recycle(int u) {
        if (!u || --tr[u].refcnt) return;
        if (tr[u].ls) {
            recycle(tr[u].ls);
            recycle(tr[u].rs);
        }
        bin.push_back(u);
    }

    int clone(int u) {
        if (!u) return 0;
        Node old = tr[u];
        int v = alloc();
        tr[v] = old;
        tr[v].refcnt = 1;  // 新根只有一份引用，不能复制旧根的引用次数。
        if (old.ls) {
            ++tr[old.ls].refcnt;
            ++tr[old.rs].refcnt;
        }
        return v;
    }

    void pushup(int u) {
        Info s = info(tr[u].ls) + info(tr[u].rs);
        tr[u].w = s.len;
        tr[u].sum = s.sum;
        tr[u].moment = s.moment;
    }

    // 只修改独占的根；cut 和 split 保证返回的非空根独占。
    void multiply(int u, int factor) {
        if (!u) return;
        assert(tr[u].refcnt == 1);
        tr[u].sum = mul(tr[u].sum, factor);
        tr[u].moment = mul(tr[u].moment, factor);
        tr[u].tag = mul(tr[u].tag, factor);
    }

    // 以下 cut / merge / split 消耗传入的根，返回的根接管其引用。
    pair<int, int> cut(int u) {
        int ls = clone(tr[u].ls), rs = clone(tr[u].rs);
        multiply(ls, tr[u].tag);
        multiply(rs, tr[u].tag);
        recycle(u);
        return {ls, rs};
    }

    int node(int ls, int rs) {
        int u = alloc();
        tr[u].ls = ls;
        tr[u].rs = rs;
        pushup(u);
        return u;
    }

    bool balanced(int x, int y) const { return x < 3 * y; }

    int merge(int ls, int rs) {
        if (!ls || !rs) return ls | rs;
        if (!balanced(size(ls), size(rs))) {
            auto [a, b] = cut(ls);
            if (!balanced(size(b) + size(rs), size(a))) {
                auto [c, d] = cut(b);
                return merge(merge(a, c), merge(d, rs));
            }
            return merge(a, merge(b, rs));
        }
        if (!balanced(size(rs), size(ls))) {
            auto [a, b] = cut(rs);
            if (!balanced(size(ls) + size(a), size(b))) {
                auto [c, d] = cut(a);
                return merge(merge(ls, c), merge(d, b));
            }
            return merge(merge(ls, a), b);
        }
        return node(ls, rs);
    }

    // 分成前 k 项和剩余项；不保留传入的旧根。
    pair<int, int> split(int u, int k) {
        if (!k) return {0, u};
        if (k == size(u)) return {u, 0};
        auto [ls, rs] = cut(u);
        if (k <= size(ls)) {
            auto [a, b] = split(ls, k);
            return {a, merge(b, rs)};
        }
        auto [a, b] = split(rs, k - size(ls));
        return {merge(ls, a), b};
    }

    int build(const vector<int>& a, int l, int r) {
        if (l == r) {
            int u = alloc();
            tr[u].w = 1;
            tr[u].sum = a[l];
            return u;
        }
        int mid = (l + r) / 2;
        int ls = build(a, l, mid), rs = build(a, mid + 1, r);
        return node(ls, rs);
    }

    // 不展开周期：每轮共享倍增，最后丢弃多出的后缀。
    int repeat(int u, int len) {
        while (size(u) < len) {
            int v = clone(u);
            u = node(u, v);
        }
        auto [keep, drop] = split(u, len);
        recycle(drop);
        return keep;
    }

    int addPoint(int u, int pos, int delta) {
        if (!u || !delta) return u;
        if (size(u) == 1) {
            assert(tr[u].refcnt == 1);
            tr[u].sum = add(tr[u].sum, delta);
            return u;
        }
        auto [ls, rs] = cut(u);
        if (pos <= size(ls)) ls = addPoint(ls, pos, delta);
        else rs = addPoint(rs, pos - size(ls), delta);
        return node(ls, rs);
    }

    // 只读查询，不分裂、不复制节点；祖先乘法标记作用于返回值。
    Info prefix(int u, int len) const {
        if (!len) return {};
        if (len == size(u)) return info(u);
        int ls = tr[u].ls, rs = tr[u].rs;
        Info ans;
        if (len <= size(ls)) ans = prefix(ls, len);
        else ans = info(ls) + prefix(rs, len - size(ls));
        ans.sum = mul(ans.sum, tr[u].tag);
        ans.moment = mul(ans.moment, tr[u].tag);
        return ans;
    }
};

struct Calibration {
    WBLT tree;
    int root;

    explicit Calibration(const vector<int>& difference)
        : tree(int(difference.size()) - 1),
          root(tree.build(difference, 1, int(difference.size()) - 1)) {}

    // d[l+1..r] 改成 d[l+1..l+k] 的周期延拓，d[l] 保持不变。
    void calibrate(int l, int r, int k) {
        int len = r - l;
        if (k == len) return;
        auto [left, rest] = tree.split(root, l);
        auto [middle, right] = tree.split(rest, len);
        int oldSum = tree.info(middle).sum;

        auto [seed, drop] = tree.split(middle, k);
        tree.recycle(drop);
        middle = tree.repeat(seed, len);

        // 补偿 d[r+1]，保证区间外的原数组值不变。
        int correction = sub(oldSum, tree.info(middle).sum);
        right = tree.addPoint(right, 1, correction);
        root = tree.merge(left, tree.merge(middle, right));
    }

    void affine(int l, int r, int u, int v) {
        auto [rest, right] = tree.split(root, r);
        auto [left, middle] = tree.split(rest, l - 1);
        int before = tree.info(left).sum;             // 旧 a[l-1]
        int last = add(before, tree.info(middle).sum); // 旧 a[r]
        int w = sub(u, 1);

        tree.multiply(middle, u);
        middle = tree.addPoint(middle, 1, add(mul(w, before), v));
        right = tree.addPoint(right, 1, sub(0, add(mul(w, last), v)));
        root = tree.merge(left, tree.merge(middle, right));
    }

    int prefixSum(int len) const {
        Info s = tree.prefix(root, len);
        return sub(mul(len, s.sum), s.moment);
    }

    int query(int l, int r) const { return sub(prefixSum(r), prefixSum(l - 1)); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n;
    vector<int> d(n + 1);
    for (int i = 1, previous = 0, value; i <= n; ++i) {
        cin >> value;
        d[i] = sub(value, previous);
        previous = value;
    }
    Calibration solution(d);
    cin >> m;
    while (m--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            int k;
            cin >> k;
            solution.calibrate(l, r, k);
        } else if (op == 2) {
            int u, v;
            cin >> u >> v;
            solution.affine(l, r, u, v);
        } else {
            cout << solution.query(l, r) << '\n';
        }
    }
    return 0;
}
