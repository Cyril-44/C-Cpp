#include <bits/stdc++.h>
constexpr int N = 1000005;
int n;
struct Mint {
    constexpr static int MOD = 998244353;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        while (n) {
            if (n & 1) res *= b;
            b *= b, n >>= 1;
        }
        return res;
    }
    Mint inv() const { return pow(MOD - 2); }
    int get() const { return val; }
    Mint(int v = 0) : val(v) {}
private: int val;
};

int tr[N], p[N], q[N], s2[N], cnt[N], zero[N];
Mint i2;

void upd(int x) { for (; x <= n; x += x & -x) tr[x]++; }
int qry(int x) { int r = 0; for (; x > 0; x -= x & -x) r += tr[x]; return r; }

// E[ #{ i<j : s_i < s_j } ]，s 中 -1 表示未知，c[v] = 缺失值中 <=v 的个数
Mint calc(const int *s, const int *c) {
    std::fill(tr, tr + n + 1, 0);
    int k = 0;
    for (int i = 1; i <= n; i++) k += s[i] == -1;
    Mint ik = k ? Mint(k).inv() : Mint(0);
    Mint res, W;
    int u = 0;  // 前面未知的个数
    for (int j = 1; j <= n; j++) {
        if (s[j] != -1) {
            res += Mint(qry(s[j] - 1));                       // 已知-已知
            res += Mint(u) * Mint(c[s[j]]) * ik;              // 未知-已知
            W += Mint(k - c[s[j]]);                           // 供 已知-未知 使用
            upd(s[j]);
        } else {
            res += W * ik;                                    // 已知-未知
            ++u;
        }
    }
    res += Mint((long long)k * (k - 1) / 2 % Mint::MOD) * i2; // 未知-未知
    return res;
}

int main() {
    scanf("%d", &n);
    i2 = Mint(2).inv();
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]), cnt[i] = 1;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &q[i]);
        if (~q[i]) cnt[q[i]] = 0;
    }
    for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
    for (int i = 1; i <= n; i++) s2[p[i]] = q[i];

    Mint A = calc(p, zero);   // 下标 vs p
    Mint B = calc(q, cnt);    // 下标 vs q
    Mint C = calc(s2, cnt);   // p vs q
    Mint T = Mint(n) * Mint(n - 1) * i2;
    printf("%d\n", ((A + B + C - T) * i2).get());
}