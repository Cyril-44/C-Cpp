#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;
const int INF = 1e9; // 足够的极值哨兵

int a[N], pre[N], n;
set<int> p[N];

inline int pp(int x) {
    return pre[*p[x].begin()];
}

struct SegTr { // Maintain v - s[v] - pre[p[v]]
    struct Data {
        int max, maxpos, min, minpos, cnt; // cnt: 处于 S 集合中数的个数
        void put(int val, int pos) {
            maxpos = minpos = pos;
            max = -INF;
            min = INF;
            if (val > 0)
                min = val, cnt = 0;
            else
                max = val, cnt = 1;
        }
        friend Data operator+(const Data &l, const Data &r) {
            Data res;
            if (l.max >= r.max)
                res.max = l.max, res.maxpos = l.maxpos;
            else
                res.max = r.max, res.maxpos = r.maxpos;

            if (l.min <= r.min)
                res.min = l.min, res.minpos = l.minpos;
            else
                res.min = r.min, res.minpos = r.minpos;

            res.cnt = l.cnt + r.cnt;
            return res;
        }
    };

    struct Node {
        Data dat;
        int add;
        void pull(int ad) { add += ad, dat.min += ad, dat.max += ad; }
    } tr[N << 2];

    void pushdown(int u) {
        if (tr[u].add) {
            tr[u << 1].pull(tr[u].add);
            tr[u << 1 | 1].pull(tr[u].add);
            tr[u].add = 0;
        }
    }

    void pushup(int u) { tr[u].dat = tr[u << 1].dat + tr[u << 1 | 1].dat; }

    int P, L, R, X;

    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u << 1, l, mid);
        if (mid < R) upd(u << 1 | 1, mid + 1, r);
        pushup(u);
    }

    void mdf(int u, int l, int r) {
        if (l == r) return tr[u].dat.put(X, l);
        int mid = l + r >> 1;
        pushdown(u);
        if (P <= mid)
            mdf(u << 1, l, mid);
        else
            mdf(u << 1 | 1, mid + 1, r);
        pushup(u);
    }

    Data inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].dat;
        int mid = l + r >> 1;
        pushdown(u);
        if (R <= mid) return inq(u << 1, l, mid);
        if (mid < L) return inq(u << 1 | 1, mid + 1, r);
        return inq(u << 1, l, mid) + inq(u << 1 | 1, mid + 1, r);
    }

  public:
    void build(int u = 1, int l = 1, int r = n) {
        if (l == r) {
            // 修正 1：对于 l > pre[n]，初始权值上限为 1
            int init_val = min(l - pre[n], 1);
            return tr[u].dat.put(init_val, l);
        }
        int mid = l + r >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }

    void update(int v, int pre_pp, int now_pp) {
        if (pre_pp == now_pp) return;

        // 修正 2：直接从线段树查询 v 的当前权值 pre_v
        L = v;
        R = v;
        Data cur = inq(1, 1, n);
        int pre_v = (cur.cnt == 1 ? cur.max : cur.min);
        int now_v = pre_v - (now_pp - pre_pp);

        P = v;
        X = now_v;
        mdf(1, 1, n);

        if (v == n) return;

        if (pre_v > 0 && now_v <= 0) { // v 进入 S，后缀权值 +1
            L = v + 1;
            R = n;
            Data res = inq(1, 1, n);
            if (res.max == 0) { // 最小的退出 S 的元素 x
                int x = res.maxpos;
                P = x;
                X = 1;
                mdf(1, 1, n);
                L = v + 1;
                R = x - 1;
                X = 1;
                if (L <= R) upd(1, 1, n);
            } else {
                L = v + 1;
                R = n;
                X = 1;
                upd(1, 1, n);
            }
        }

        if (pre_v <= 0 && now_v > 0) { // v 离开 S，后缀权值 -1
            L = v + 1;
            R = n;
            Data res = inq(1, 1, n);
            if (res.min == 1) { // 最小的进入 S 的元素 x
                int x = res.minpos;
                P = x;
                X = 0;
                mdf(1, 1, n);
                L = v + 1;
                R = x - 1;
                X = -1;
                if (L <= R) upd(1, 1, n);
            } else {
                L = v + 1;
                R = n;
                X = -1;
                upd(1, 1, n);
            }
        }
    }

    int search(int num) {
        int u = 1, l = 1, r = n;
        while (l < r) {
            int mid = l + r >> 1;
            pushdown(u);
            if (tr[u << 1].dat.cnt >= num)
                u = u << 1, r = mid;
            else
                num -= tr[u << 1].dat.cnt, u = u << 1 | 1, l = mid + 1;
        }
        return l;
    }
} f;

#define Oper(pos, val, op)                                                                                             \
    do {                                                                                                               \
        if (~val) {                                                                                                    \
            int pre_pp = pp(val);                                                                                      \
            p[val].op(pos);                                                                                            \
            int now_pp = pp(val);                                                                                      \
            if (now_pp != pre_pp) f.update(val, pre_pp, now_pp);                                                       \
        }                                                                                                              \
    } while (0)

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    int q;
    cin >> q;

    fill(p + 1, p + 1 + n, set<int>{n + 1});
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pre[i] = pre[i - 1] + !a[i];
    }
    pre[n + 1] = pre[n];

    f.build();

    for (int _q = 1; _q <= q; _q++) {
        int x, k, y;
        cin >> x >> k >> y;
        Oper(x, a[x], erase);
        a[x] = k;
        Oper(x, k, insert);

        cout << (a[y] == 0 ? f.search(pre[y]) : a[y]) << '\n';
        assert(f.tr[1].dat.cnt == pre[n]);
    }
    return 0;
}