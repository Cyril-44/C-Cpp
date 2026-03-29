#include <bits/stdc++.h>
using namespace std;
const int N = 400005, INF = 2e9;
int n, m, q;
vector<int> adds[N];
struct Qry { int R, id; };
vector<Qry> qrys[N];
int ans[N];
struct SegTr {
int minv[N << 2], maxv[N << 2], lz[N << 2];
void build(int u, int l, int r) {
    lz[u] = INF;
    if (l == r) {
        minv[u] = maxv[u] = INF;
        return;
    }
    int mid = (l + r) / 2;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    minv[u] = min(minv[u << 1], minv[u << 1 | 1]);
    maxv[u] = max(maxv[u << 1], maxv[u << 1 | 1]);
}
void pushdown(int u, int l, int r) {
    if (lz[u] != INF) {
        minv[u] = min(minv[u], lz[u]);
        maxv[u] = min(maxv[u], lz[u]);
        if (l != r) {
            lz[u << 1] = min(lz[u << 1], lz[u]);
            lz[u << 1 | 1] = min(lz[u << 1 | 1], lz[u]);
        }
        lz[u] = INF;
    }
}
int ql, qr, val;
void upd(int u, int l, int r) {
    if (ql <= l && r <= qr) {
        lz[u] = min(lz[u], val);
        pushdown(u, l, r);
        return;
    }
    int mid = (l + r) / 2;
    pushdown(u, l, r);
    if (ql <= mid) upd(u * 2, l, mid);
    if (mid < qr) upd(u * 2 + 1, mid + 1, r);
    minv[u] = min(minv[u * 2], minv[u * 2 + 1]);
    maxv[u] = max(maxv[u * 2], maxv[u * 2 + 1]);
}
inline void update(int L, int R, int v) {
    ql = L, qr = R, val = v;
    upd(1, 1, m);
}
int que(int u, int l, int r) {
    pushdown(u, l, r);
    if (qr < l || ql > r) return 0;
    if (ql <= l && r <= qr) {
        if (maxv[u] <= val) return r - l + 1;
        if (minv[u] > val) return 0;
        if (l == r) return (minv[u] <= val ? 1 : 0);
        int mid = (l + r) / 2;
        return que(u * 2, l, mid) + que(u * 2 + 1, mid + 1, r);
    }
    int mid = (l + r) / 2;
    return que(u * 2, l, mid) + que(u * 2 + 1, mid + 1, r);
}
inline int inquire(int L, int R, int v) {
    ql = L, qr = R, val = v;
    return que(1, 1, m);
}
} seg;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> q;
    for (int i = 1; i <= n; ++i) {
        int l, r;
        cin >> l >> r;
        if (l <= m && r <= m) adds[l].push_back(r);
    }
    for (int i = 0; i < q; ++i) {
        int L, R;
        cin >> L >> R;
        if (L <= m) qrys[L].push_back({R, i});
    }
    seg.build(1, 1, m);
    for (int L = m; L >= 1; --L) {
        for (int r : adds[L]) {
            if (r >= L) {
                seg.update(L, r, r);
            }
        }
        for (auto &qu : qrys[L]) {
            int R = qu.R;
            if (R < L) {
                ans[qu.id] = 0;
            } else {
                ans[qu.id] = seg.inquire(L, R, R);
            }
        }
    }
    for (int i = 0; i < q; ++i) {
        cout << ans[i] << '\n';
    }
    return 0;
}