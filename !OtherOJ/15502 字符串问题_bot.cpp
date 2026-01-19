#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e6 + 10;
const int MOD = 998244353;

int n;
string s;
ll f[N], ans[N];

struct SuffixArray {
    int n;
    vector<int> sa, rk, tmp, height;
    vector<vector<int>> st;
    vector<int> lg2;
    SuffixArray() {}
    SuffixArray(const string &str, int sigma = 256) { build(str, sigma); }

    void build(const string &str, int sigma = 256) {
        n = (int)str.size();
        sa.assign(n, 0);
        rk.assign(n, 0);
        tmp.assign(n, 0);
        for (int i = 0; i < n; ++i) rk[i] = (unsigned char)str[i], sa[i] = i;
        for (int k = 1;; k <<= 1) {
            auto cmp = [&](int a, int b) {
                if (rk[a] != rk[b]) return rk[a] < rk[b];
                int ra = a + k < n ? rk[a + k] : -1;
                int rb = b + k < n ? rk[b + k] : -1;
                return ra < rb;
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) tmp[sa[i]] = tmp[sa[i-1]] + cmp(sa[i-1], sa[i]);
            for (int i = 0; i < n; ++i) rk[i] = tmp[i];
            if (rk[sa[n-1]] == n-1) break;
        }
        build_height(str);
        build_rmq();
    }

    void build_height(const string &str) {
        height.assign(n, 0);
        vector<int> inv(n);
        for (int i = 0; i < n; ++i) inv[sa[i]] = i;
        int h = 0;
        for (int i = 0; i < n; ++i) {
            if (inv[i] > 0) {
                int j = sa[inv[i] - 1];
                while (i + h < n && j + h < n && str[i + h] == str[j + h]) ++h;
                height[inv[i]] = h;
                if (h) --h;
            }
        }
        rk = inv; // reuse rk as inverse (rank)
    }

    void build_rmq() {
        int m = n;
        lg2.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg2[i] = lg2[i >> 1] + 1;
        int K = lg2[m] + 1;
        st.assign(K, vector<int>(m));
        for (int i = 0; i < m; ++i) st[0][i] = height[i];
        for (int k = 1; k < K; ++k) {
            for (int i = 0; i + (1 << k) <= m; ++i)
                st[k][i] = min(st[k-1][i], st[k-1][i + (1 << (k-1))]);
        }
    }

    int lcp(int x, int y) {
        if (x == y) return n - x;
        int rx = rk[x], ry = rk[y];
        if (rx > ry) swap(rx, ry);
        ++rx;
        int len = ry - rx + 1;
        int k = lg2[len];
        return min(st[k][rx], st[k][ry - (1<<k) + 1]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> s;
    for (int i = 1; i <= n; ++i) cin >> f[i];

    for (int i = 1; i <= n; ++i) {
        for (int j = i + i; j <= n; j += i) {
            f[j] = (f[j] - f[i]) % MOD;
            if (f[j] < 0) f[j] += MOD;
        }
    }
    /* 等价于
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; j += i)
            g[j] = (g[j] + mu[i] * f[j / i]) % MOD;
    */

    SuffixArray sa(s);

    string rs = s;
    reverse(rs.begin(), rs.end());
    SuffixArray rsa(rs);

    auto LCP = [&](int x, int y)->int {
        if (x < 1 || y < 1 || x > n || y > n) return 0;
        return sa.lcp(x - 1, y - 1);
    };
    auto LCS = [&](int x, int y)->int {
        if (x < 1 || y < 1 || x > n || y > n) return 0;
        return rsa.lcp(n - x, n - y);
    };

    // 枚举 d
    for (int d = 1; d <= n; ++d) {
        ans[d] = (ans[d] + f[1]) % MOD;
        vector<pair<int,int>> segs;
        for (int i = d; i + d <= n; i += d) {
            int lp = LCP(i, i + d);
            int ls = LCS(i, i + d);
            if (lp > 0 && ls > 0) {
                int L = i - ls + 1;
                int R = i + lp - 1;
                if (segs.empty()) segs.emplace_back(L, R);
                else {
                    if (segs.back().second >= L - 1) segs.back().second = max(segs.back().second, R);
                    else segs.emplace_back(L, R);
                }
            }
        }
        for (auto &pr : segs) {
            int L = pr.first, R = pr.second;
            int len = R - L + 1;
            for (int o = d; o <= len; o += d) {
                int idx = o / d + 1;
                if (L + o - 1 + d <= n) {
                    ans[L + o - 1 + d] = (ans[L + o - 1 + d] + f[idx]) % MOD;
                }
                if (R + d + 1 <= n) {
                    ans[R + d + 1] = (ans[R + d + 1] - f[idx]) % MOD;
                    if (ans[R + d + 1] < 0) ans[R + d + 1] += MOD;
                }
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        ans[i] = (ans[i] + ans[i-1]) % MOD;
        if (ans[i] < 0) ans[i] += MOD;
        cout << ans[i] << ' ';
    }
    return 0;
}
