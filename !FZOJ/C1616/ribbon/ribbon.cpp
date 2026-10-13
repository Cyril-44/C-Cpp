#include <bits/stdc++.h>
#include <sys/types.h>
constexpr int N = 200005, MOD = 998244353;
#ifdef CLANGD
#define N 25
#endif
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
private: int val;
};
int n, p[N], c[N], fpgt[N], fgt[N], fplt[N], flt[N], bpgt[N], bgt[N], bplt[N], blt[N];
template<class Comp = std::less<int>>
inline void calc(const int *a, int *b, const bool inv = false) { // b[i] 记录前面/后面第一个比 a[i] 小/大的位置
    static int sta[N];
    int top = 0;
    auto solveOnce = [&](int i) {
        while (top && !Comp{}(a[sta[top]], a[i])) --top;
        b[i] = sta[top], sta[++top] = i;
    };
    if (inv) for (int i = n; i >= 1; i--) solveOnce(i);
    else     for (int i = 1; i <= n; i++) solveOnce(i);
}
inline void fillto(const int *a, const int *p, int *b, const int df = 0) {
    for (int i = 1; i <= n; i++) b[i] = p[i] ? a[p[i]] : df;
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &c[i]);
    calc(p, fplt), fillto(p, fplt, flt);
    calc<std::greater<int>>(p, fpgt), fillto(p, fpgt, fgt, n+1);
    calc(p, bplt, 1), fillto(p, bplt, blt);
    calc<std::greater<int>>(p, bpgt), fillto(p, bpgt, bgt, n+1);
    static int sta[N]; int top = 0;
    for (int i = 1; i <= n; i++) {
        while (top && flt[sta[top]] > [i]) --top;
    }
}