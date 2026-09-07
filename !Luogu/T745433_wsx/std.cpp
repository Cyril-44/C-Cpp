#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
static const int64 INF = (1LL << 62);

struct RMQ {
    int n, lg;
    vector<vector<int64>> st;

    RMQ() {}
    RMQ(const vector<int64>& a) { build(a); }

    void build(const vector<int64>& a) {
        n = (int)a.size();
        lg = 1;
        while ((1 << lg) <= n) ++lg;
        st.assign(lg, vector<int64>(n));
        st[0] = a;
        for (int j = 1; j < lg; ++j) {
            int len = 1 << j;
            int half = len >> 1;
            for (int i = 0; i + len <= n; ++i) {
                st[j][i] = min(st[j - 1][i], st[j - 1][i + half]);
            }
        }
    }

    int64 query(int l, int r) const {
        if (l > r) return INF;
        int len = r - l + 1;
        int j = 31 - __builtin_clz(len);
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        unsigned long long s, t;
        int k;
        cin >> s >> t >> k;
        vector<int64> a(k);
        for (int i = 0; i < k; ++i) cin >> a[i];

        if (s == t) {
            cout << 0 << '\n';
            continue;
        }

        int A = __builtin_popcountll(s);
        int B = __builtin_popcountll(t);
        int C = __builtin_popcountll(s & t);

        int m = A - C;                 // s only
        int n = B - C;                 // t only
        int d = k - A - B + C;         // neither
        int unionST = A + B - C;       // |s ∪ t|

        RMQ rmq(a);

        int64 ans = a[C];              // 1 段

        // 2 段
        for (int p = 0; p <= min(A, k - 1); ++p) {
            int L = max(0, p - m);
            int R = min(k - 1, n + min(C, p));
            if (L <= R) {
                ans = min(ans, a[p] + rmq.query(L, R));
            }
        }

        // 3 段
        for (int p = 0; p <= min(A, k - 1); ++p) {
            for (int r = 0; r <= min(B, k - 1); ++r) {
                int L = max(0, p + r - unionST);
                int U;
                if (p <= m && r <= n) {
                    U = d + p + r;
                } else if (p <= m && r > n) {
                    U = d + p + n;
                } else if (p > m && r <= n) {
                    U = d + m + r;
                } else {
                    U = d + m + n + min({C, p - m, r - n});
                }
                U = min(U, k - 1);
                if (L <= U) {
                    ans = min(ans, a[p] + a[r] + rmq.query(L, U));
                }
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
/*

1
12 8 4
96 707 34 313
*/