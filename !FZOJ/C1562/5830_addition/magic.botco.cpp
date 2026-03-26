#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;

long long mod_pow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

struct Matrix {
    long long a00, a01, a10, a11;
};

Matrix mul(const Matrix &x, const Matrix &y) {
    Matrix r;
    r.a00 = (x.a00 * y.a00 + x.a01 * y.a10) % MOD;
    r.a01 = (x.a00 * y.a01 + x.a01 * y.a11) % MOD;
    r.a10 = (x.a10 * y.a00 + x.a11 * y.a10) % MOD;
    r.a11 = (x.a10 * y.a01 + x.a11 * y.a11) % MOD;
    return r;
}

Matrix mat_pow(Matrix base, long long e) {
    Matrix r{1, 0, 0, 1}; // identity
    while (e) {
        if (e & 1) r = mul(r, base);
        base = mul(base, base);
        e >>= 1;
    }
    return r;
}

struct SegTree {
    struct Node {
        long long sum;
        long long mul, add;
    };
    int n;
    vector<Node> st;

    SegTree(int n) : n(n) {
        st.resize(4 * n + 4);
        build(1, 1, n);
    }

    void build(int p, int l, int r) {
        st[p].sum = 0;
        st[p].mul = 1;
        st[p].add = 0;
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);
    }

    void apply(int p, int l, int r, long long mulv, long long addv) {
        st[p].sum = (st[p].sum * mulv + addv * (r - l + 1)) % MOD;
        st[p].mul = st[p].mul * mulv % MOD;
        st[p].add = (st[p].add * mulv + addv) % MOD;
    }

    void push(int p, int l, int r) {
        if (st[p].mul == 1 && st[p].add == 0) return;
        int mid = (l + r) >> 1;
        apply(p << 1, l, mid, st[p].mul, st[p].add);
        apply(p << 1 | 1, mid + 1, r, st[p].mul, st[p].add);
        st[p].mul = 1;
        st[p].add = 0;
    }

    void updd(int p, int l, int r, int ql, int qr, long long mulv, long long addv) {
        if (ql <= l && r <= qr) {
            apply(p, l, r, mulv, addv);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) updd(p << 1, l, mid, ql, qr, mulv, addv);
        if (qr > mid) updd(p << 1 | 1, mid + 1, r, ql, qr, mulv, addv);
        st[p].sum = (st[p << 1].sum + st[p << 1 | 1].sum) % MOD;
    }

    long long quee(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return st[p].sum;
        push(p, l, r);
        int mid = (l + r) >> 1;
        long long res = 0;
        if (ql <= mid) res = (res + quee(p << 1, l, mid, ql, qr)) % MOD;
        if (qr > mid) res = (res + quee(p << 1 | 1, mid + 1, r, ql, qr)) % MOD;
        return res;
    }

    void range_update(int l, int r, long long mulv, long long addv) {
        updd(1, 1, n, l, r, mulv, addv);
    }

    long long range_query(int l, int r) {
        return quee(1, 1, n, l, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_id;
    if (!(cin >> test_id)) return 0;

    int n, m;
    cin >> n >> m;

    vector<int> L(n + 1), R(n + 1);
    vector<long long> C(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> L[i] >> R[i] >> C[i];
    }

    SegTree seg(m);

    long long A = 1;

    for (int i = 1; i <= n; ++i) {
        int l = L[i], r = R[i];
        long long ci = C[i];
        int k = r - l + 1;
        long long invk = mod_pow(k, MOD - 2);
        
        long long S_in = seg.range_query(l, r);

        Matrix M;
        M.a00 = 2 % MOD;
        M.a01 = (MOD - invk) % MOD;
        M.a10 = 1;
        M.a11 = (1 - invk + MOD) % MOD;

        Matrix P = mat_pow(M, ci);

        long long A_out = (P.a00 * A + P.a01 * S_in) % MOD;
        long long S_out = (P.a10 * A + P.a11 * S_in) % MOD;

        long long alpha = mod_pow((1 - invk + MOD) % MOD, ci);
        long long beta = (S_out - alpha * S_in) % MOD;
        if (beta < 0) beta += MOD;
        beta = beta * invk % MOD;

        seg.range_update(l, r, alpha, beta);

        A = A_out;
    }

    A %= MOD;
    if (A < 0) A += MOD;
    cout << A << '\n';

    return 0;
}
