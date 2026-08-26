#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

constexpr int N = 400004;
constexpr i128 INF = 1000000000000000000LL; // > n

int n, a[N], h[N];

struct Node {
    ll min_req = 0;
    ll S = 0;
    i128 fin = 0;   // starting from min_req, capped at INF
    int len = 0;
};

Node tr[N * 4];

Node merge(const Node& high, const Node& low) { // high 先处理（值更大）
    Node res;
    res.len = high.len + low.len;
    res.S = high.S + low.S;

    // 计算合并后的 min_req
    i128 excess = 0;
    if (high.fin < low.min_req) {
        if (high.len >= 60) {
            excess = 1;
        } else {
            i128 mul = i128(1) << high.len;
            excess = (low.min_req - high.fin + mul - 1) / mul; // ceil
        }
    }
    res.min_req = high.min_req + (ll)min(excess, (i128)n + 1);

    // 从新 min_req 得到最终 fin
    i128 after_h;
    if (high.len >= 60) {
        after_h = (excess > 0 ? INF : high.fin);
    } else {
        after_h = high.fin + excess * (i128(1) << high.len);
    }
    if (after_h > INF) after_h = INF;

    if (after_h < low.min_req) after_h = low.min_req; // 数值误差保护

    i128 low_ex = after_h - low.min_req;
    i128 final;
    if (low.len >= 60) {
        final = (low_ex > 0 ? INF : low.fin);
    } else {
        final = low.fin + low_ex * (i128(1) << low.len);
    }
    if (final > INF) final = INF;
    res.fin = final;
    return res;
}

void build(int p, int l, int r) {
    if (l == r) {
        // 叶子：频率 h[l]
        ll hh = h[l];
        tr[p].min_req = hh;
        tr[p].S = hh;
        tr[p].fin = hh;          // 2*hh - hh
        tr[p].len = 1;
        return;
    }
    int mid = (l + r) >> 1;
    build(p << 1, l, mid);
    build(p << 1 | 1, mid + 1, r);
    // 右儿子是高值，左儿子是低值
    tr[p] = merge(tr[p << 1 | 1], tr[p << 1]);
}

void update(int p, int l, int r, int pos) {
    if (l == r) {
        ll hh = h[l];
        tr[p].min_req = hh;
        tr[p].S = hh;
        tr[p].fin = hh;
        tr[p].len = 1;
        return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) update(p << 1, l, mid, pos);
    else update(p << 1 | 1, mid + 1, r, pos);
    tr[p] = merge(tr[p << 1 | 1], tr[p << 1]);
}

// 在区间 [ql, qr] 上从高到低应用变换（递归，带提前终止）
void apply(int p, int l, int r, int ql, int qr, i128& req, i128& all) {
    if (qr < l || r < ql || req > all) return;
    if (ql <= l && r <= qr) {
        if (req >= tr[p].min_req) {
            all -= tr[p].S;
            i128 excess = req - tr[p].min_req;
            int len = tr[p].len;
            if (len >= 60) {
                req = (excess > 0 ? all + 1 : tr[p].fin);
            } else {
                req = tr[p].fin + excess * (i128(1) << len);
            }
            if (req > all) req = all + 1;
            return;
        }
        // 无法满取，必须下沉
        if (l == r) {
            // 叶子强制精确计算
            i128 take = min((i128)h[l], req);
            all -= take;
            req = 2 * req - take;
            if (req > all) req = all + 1;
            return;
        }
    }
    if (l == r) return; // 已处理
    int mid = (l + r) >> 1;
    // 先右（高值）后左（低值）
    apply(p << 1 | 1, mid + 1, r, ql, qr, req, all);
    apply(p << 1, l, mid, ql, qr, req, all);
}

bool check(int sz, int val) {
    if (val <= 1) return true;
    i128 req = 1, all = sz;
    apply(1, 1, n, 1, val - 1, req, all);
    return all >= req;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);

    // 初始全 0
    build(1, 1, n);

    int val = 1;
    ++h[a[1]];
    update(1, 1, n, a[1]);
    printf("%d", max(1, a[1]));

    for (int i = 2; i <= n; ++i) {
        int x = a[i];
        ++h[x];
        update(1, 1, n, x);
        while (val < i && check(i, val + 1)) ++val;
        printf(" %d", val);
    }
    putchar('\n');
    return 0;
}