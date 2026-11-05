#include <bits/stdc++.h>
constexpr int M = 205, MOD = 10007;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < MOD) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1u * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint pow(int n) {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v % MOD) {}
    Mint(uint64_t v) : val(v % MOD) {}
private: int val;
};
char a[M], t[M];
int n, m;
std::unordered_set<uint64_t> st;
inline void count() {
    uint64_t hash = 0;
    for (int i = 0, j = n+m-1; i < n+m; i++, j--) {
        if (t[i] != t[j]) return;
        hash = hash * 26 + (t[i] - 'a');
    }
    st.insert(hash);
}
void dfs(int i, int j) {
    if (i >= m && j >= n) return count();
    if (i < m) t[i+j] = a[i], dfs(i+1, j);
    if (j < n) 
        for (char c = 'a'; c <= 'z'; c++)
            t[i+j] = c, dfs(i, j+1);
}
int main() {
    freopen("palindrome.in", "r", stdin);
    freopen("palindrome.out", "w", stdout);
    scanf("%s %d", a, &n);
    m = strlen(a);
    if (m <= 10 && n <= 3) dfs(0, 0);
    printf("%d\n", Mint(st.size()));
}