#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
constexpr int N = 305, MOD = (int)1e9 + 7;
struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    template<typename T> inline explicit operator T() const { return static_cast<T>(val); }
    inline ModInt& operator+=(ModInt o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline ModInt& operator-=(ModInt o) { if ((val -= o.val) < 0) val += MOD;    return *this; }
    inline ModInt& operator*=(ModInt o) { val = 1ull * val * o.val % MOD;        return *this; }
    inline ModInt operator+(ModInt o) const { return ModInt(*this) += o; }
    inline ModInt operator-(ModInt o) const { return ModInt(*this) -= o; }
    inline ModInt operator*(ModInt o) const { return ModInt(*this) *= o; }
    private: int val;
} f[N][N][N][2];
#ifdef ONLINE_JUDGE
#define fprintf(...) ((void)0)
#endif
int n, m, x, y;
int b[N * 2];
std::vector<int> g[N];
struct BruteForce {
    std::map<std::multiset<int>, int> st;
    std::multiset<int> cur;
    int a[N];
    ModInt ans, res;
    inline int calc(int u, int mx) {
        mx = std::max(mx, a[u]);
        int smx = a[u];
        for (int v : g[u]) smx = std::max(smx, calc(v, mx));
        res *= b[mx * x + smx * y];
        cur.insert(mx * x + smx * y);
        return smx;
    }
    void dfs(int i) {
        if (i > n) res = 1, cur.clear(), calc(1, 0), st[cur]++, ans += res;
        else For (v, 1, m) a[i] = v, dfs(i + 1);
    }
};
void dfs(int u) {
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= m; j++) {
            f[u][i][j][0] = f[u][i][j][1] = 1;
            if (i == j) f[u][i][j][1] = b[x*i + y*j];
        }
    for (int v : g[u]) {
        dfs(v);
        ModInt tp[N][N][2];
        For (au, 1, m) {
            For (i, au, m)
                For (j, au, m) {
                    if (au == i && au == j) { // >=i, <=j
                        For (ti, i+1, m)
                            For (tj, 1, j) {
                                fprintf(stderr, "$1A Add tp[%d][%d][1] <-- %d\n", i, j, (int)f[v][ti][tj][1]);
                                tp[i][j][1] += f[v][ti][tj][1];
                            }
                        For (tj, 1, j) {
                            fprintf(stderr, "$1B Add tp[%d][%d][1] <-- %d\n", i, j, int(f[v][i][tj][0] + f[v][i][tj][1]));
                            tp[i][j][1] += f[v][i][tj][0] + f[v][i][tj][1];
                        }
                    } else if (au == i) { // >=i, ==j
                        For (ti, i+1, m) {
                            fprintf(stderr, "$2A Add tp[%d][%d][1] <-- %d(f[%d][%d][1])\n", i, j, (int)f[v][ti][j][1], ti, j);
                            tp[i][j][1] += f[v][ti][j][1];
                        }
                        fprintf(stderr, "$2B Add tp[%d][%d][1] <-- %d(f[%d][%d][0]+[1])\n", i, j, int(f[v][i][j][0] + f[v][i][j][1]), i, j);
                        tp[i][j][1] += f[v][i][j][0] + f[v][i][j][1];
                    } else if (au == j) { // >=i, <=j
                        For (tj, 1, j) {
                            fprintf(stderr, "$3 Add tp[%d][%d][0] <-- %d\n", i, j, int(f[v][i][tj][0] + f[v][i][tj][1]));
                            tp[i][j][0] += f[v][i][tj][0] + f[v][i][tj][1];
                        }
                    } else { // ==i, ==j
                        fprintf(stderr, "$4 Add tp[%d][%d][0] <-- %d\n", i, j, int(f[v][i][j][0] + f[v][i][j][1]));
                        tp[i][j][0] += f[v][i][j][0] + f[v][i][j][1];
                    }
                }
        }
        For (i, 1, m)
            For (j, 1, m)
                f[u][i][j][0] *= tp[i][j][0],
                f[u][i][j][1] *= tp[i][j][1];
        For (i, 1, m)
            For (j, 1, m)
                fprintf(stderr, "*(%d,%d)%c", int(tp[i][j][0]), int(tp[i][j][1]), j==m?'\n':' ');
    }
    For (i, 1, m)
        For (j, 1, m)
            fprintf(stderr, "(%d,%d)%c", int(f[u][i][j][0]), int(f[u][i][j][1]), j==m?'\n':' ');
}
int main() {
    scanf("%d%d%d%d", &n, &m, &x, &y);
    for (int i = 1; i <= 2*m; i++) scanf("%d", &b[i]);
    for (int i = 2, p; i <= n; i++) scanf("%d", &p), g[p].push_back(i);
    if (n <= 7 && m <= 7) {
        BruteForce sol;
        sol.dfs(1);
        printf("%d\n", int(sol.ans));
        for (const auto &[s, id] : sol.st) {
            fprintf(stderr, "cnt=%d: ", id);
            for (int i : s) fprintf(stderr, "%d ", i);
            fputc('\n', stderr);
        }
    } else {
        dfs(1);
        ModInt ans;
        For (i, 1, m)
            For (j, 1, m)
                ans += f[1][i][j][1];
        printf("%d\n", int(ans));
    }
    return 0;
}