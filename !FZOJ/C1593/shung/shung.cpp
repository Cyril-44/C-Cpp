#include <cstdio>
#include <algorithm>
constexpr int N = 200002;
int p[N], n;
struct BF {
    static constexpr int N = 2002;
    short f[N][N];
    struct BIT {
        int tr[N];
        void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] = std::max(tr[p], x); }
        int operator()(int p) const { int res = 0; for (; p > 0; p -= p & -p) res = std::max(res, tr[p]); return res; }
    } fl[N], fr[N];
    void operator()() {
        for (int i = 1; i <= n; i++) {
            int x = p[i];
            for (int i = 1; i <= n; i++) {
                f[i][x] = fl[i](x) + 1;
                f[x][i] = fr[i](n-x+1) + 1;
            }
            for (int i = 1; i <= n; i++) {
                fl[i].upd(x, f[i][x]);
                fr[x].upd(n-i+1, f[i][x]);
                fr[i].upd(n-x+1, f[x][i]);
                fl[x].upd(i, f[x][i]);
            }
        }
        int ans = 0;
        for (int i = 1; i <= n; i++)
            ans = std::max(ans, std::max(fl[i](n), fr[i](n)));
        printf("%d\n", ans);
    }
};
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &p[i]);
    if (n <= 2000) BF{}();
    return 0;
}