#include <bits/stdc++.h>
constexpr int N = 1000005;
int64_t b[N];
struct BIT {
    constexpr static int M = 2e6;
    int tr[M+1];
    void upd(int p, int x) { p = M-p+1; for (; p <= M; p += p & -p) tr[p] += x; }
    int sum(int p) { p = M-p+1; int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
} f;
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int n, m;
    std::cin >> n >> m;
    for (int i = 1, ai; i <= n; i++) std::cin >> ai, f.upd(ai, 1);
    for (int i = 1; i <= m; i++) {
        int l; std::cin >> l;
        for (int j = 1, bj; j <= l; j++) {
            std::cin >> bj;
            b[i] += f.sum(bj);
        }
    }
    std::sort(b+1, b+1+m);
    int64_t now = (n+1ll) * n / 2, ans = 0;
    for (int i = 1; i <= m; i++)
        ans += now += b[i];
    std::cout << ans << '\n';
}