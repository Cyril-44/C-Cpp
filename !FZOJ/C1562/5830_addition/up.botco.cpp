#include <bits/stdc++.h>
using namespace std;

const int KMAX = 6;
const int INF_NEG = -1000000000;

struct Node {
    int f[KMAX][KMAX];
    Node() {
        for (int i = 0; i < KMAX; ++i)
            for (int j = 0; j < KMAX; ++j)
                f[i][j] = INF_NEG;
    }
};

int n, k, q;
vector<string> rowStr;

Node mergeNode(const Node &A, const Node &B, int k) {
    Node C;
    for (int i = 0; i < k; ++i)
        for (int j = i; j < k; ++j)
            C.f[i][j] = max(A.f[i][j], B.f[i][j]);
    for (int i = 0; i < k; ++i) {
        for (int j = i; j < k; ++j) {
            int best = C.f[i][j];
            for (int m = i; m <= j; ++m) {
                int v1 = A.f[i][m];
                int v2 = B.f[m][j];
                if (v1 <= INF_NEG || v2 <= INF_NEG) continue;
                best = max(best, v1 + v2);
            }
            C.f[i][j] = best;
        }
    }
    return C;
}

struct SegTree {
    int n, k;
    vector<Node> seg;
    SegTree(int n_, int k_) : n(n_), k(k_) {
        seg.resize(4 * n);
    }
    Node makeLeaf(int idx) {
        Node node;
        for (int i = 0; i < k; ++i)
            for (int j = 0; j < k; ++j)
                node.f[i][j] = INF_NEG;
        // single row: any continuous segment of 1s gives 1
        for (int i = 0; i < k; ++i) {
            if (rowStr[idx][i] == '1') {
                int j = i;
                while (j < k && rowStr[idx][j] == '1') {
                    node.f[i][j] = 1;
                    ++j;
                }
            }
        }
        return node;
    }
    void build(int p, int l, int r) {
        if (l == r) {
            seg[p] = makeLeaf(l);
            return;
        }
        int mid = (l + r) >> 1;
        build(p<<1, l, mid);
        build(p<<1|1, mid+1, r);
        seg[p] = mergeNode(seg[p<<1], seg[p<<1|1], k);
    }
    void update(int p, int l, int r, int pos) {
        if (l == r) {
            seg[p] = makeLeaf(l);
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) update(p<<1, l, mid, pos);
        else update(p<<1|1, mid+1, r, pos);
        seg[p] = mergeNode(seg[p<<1], seg[p<<1|1], k);
    }
    Node query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return seg[p];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(p<<1, l, mid, ql, qr);
        if (ql > mid) return query(p<<1|1, mid+1, r, ql, qr);
        Node left = query(p<<1, l, mid, ql, qr);
        Node right = query(p<<1|1, mid+1, r, ql, qr);
        return mergeNode(left, right, k);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    cin >> n >> k >> q;
    rowStr.assign(n, string());
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        rowStr[i] = s;
    }
    SegTree st(n, k);
    st.build(1, 0, n-1);

    while (q--) {
        char c;
        cin >> c;
        if (c == 'C') {
            int x, y;
            cin >> x >> y;
            rowStr[x][y] = (rowStr[x][y] == '1' ? '0' : '1');
            st.update(1, 0, n-1, x);
        } else if (c == 'Q') {
            int x, y, l, r;
            cin >> x >> y >> l >> r;
            Node res = st.query(1, 0, n-1, x, y);
            int ans = res.f[l][r];
            if (ans <= INF_NEG/2) ans = 0;
            cout << ans << "\n";
        }
    }
    return 0;
}
