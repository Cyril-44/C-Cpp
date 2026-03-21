#include <cstdio>
#include <cstring>
#include <algorithm>
#include <array>
#include <bitset>
#include <vector>
#pragma GCC optimize(2)
constexpr int N = 8005, INF = 0x3f3f3f3f;
std::bitset<N> a;
std::vector<int> edgs[N];
inline void umin(int &x, int y) { (x > y) && (x = y); }
/* namespace BF1 {
constexpr int N = 505;
using DPArr = std::vector<std::vector<int>>;
int ans[N], n;
inline void init(DPArr &f) { f.assign(n+1, std::vector<int>(n+1, INF)); }
DPArr dfs(int u, int pre) {
    DPArr f0, f1, g0, g1; init(f0), init(f1);
    a[u] ?  f0[1][1] = f1[0][0] = 0 : f0[0][1] = f1[1][0] = 0;
    for (int v : edgs[u]) if (v != pre) {
        auto fv = dfs(v, u); init(g0), init(g1);
        for (int k = 0; k   <= n; k++) for (int l = 0; l   <= n; l++) if (fv[k][l] != INF)
        for (int i = 0; i+k <= n; i++) for (int j = 0; j+l <= n; j++)
            umin(g0[k+i][l+j], f0[i][j] + fv[k][l]), umin(g1[k+i][l+j], f1[i][j] + fv[k][l] + l);
        f0 = g0, f1 = g1;
    }
    for (int i = 0; i <= n; i++) for (int j = 0; j <= n; j++) umin(f0[i][j], f1[i][j]);
    return f0;
}
inline void work(int n) {
    BF1::n = n; auto f = dfs(1, 0);
    for (int i = 0; i <= n; i++) {
        ans[i] = *std::min_element(f[i].begin(), f[i].end());
        if (i >= 2) ans[i] = std::min(ans[i], ans[i - 2]);
        printf("%d ", ans[i]);
    }
    putchar('\n');
}
} */
inline bool readBoolean() { int ch = getchar(); while (ch != '0' && ch != '1') ch = getchar(); return ch & 1; }

using DPA = std::vector<std::array<int, 3>>;
DPA f[N];
int sz[N], down0[N], ans[N];
/*
最后的答案一定是分成三层
最上面全是 0；中间不变；下面是 1
分别对应 DP 第三维中的 2 1 0 且状态都包含
*/
inline void init(DPA &f, int n) { f.resize(n+1); for (int i = 0; i <= n; i++) f[i].fill(INF); }
void dfs(int u, int pre) {
    sz[u] = 1, down0[u] = !a[u];
    for (int v : edgs[u]) if (v != pre) dfs(v, u);
    init(f[u], 1);
    DPA oldfu;
    if (a[u]) f[u][0][0] = f[u][0][1] = f[u][1][2] = 0;
    else      f[u][1][0] = f[u][0][1] = f[u][0][2] = 0;
    for (int v : edgs[u]) if (v != pre) {
        down0[u] += down0[v];
        DPA oldfu = f[u];
        init(f[u], sz[u] + sz[v]);
        for (int i = 0; i <= sz[u]; i++)
            for (int k = 0; k < 3; k++) if (oldfu[i][k] != INF)
                for (int j = 0; j <= sz[v]; j++)
                    for (int l = 0; l <= k; l++) { // 实际上 k,l<=1 的时候 j 就是下面的 0==>1 的数量
                        int val = f[v][j][l];
                        if ((k == 1 && l == 0 || k == 1 && l == 1) && a[u]) val += down0[v] - j;
                        umin(f[u][i+j][k], oldfu[i][k] + val);
                    }
        f[v].clear(), f[v].shrink_to_fit();
        sz[u] += sz[v];
    }
    /* printf("[%2d]", u);
    for (int i = 0; i <= sz[u]; i++) printf("#%2d%c", i, i==sz[u]?'\n':' ');
    for (int j = 0; j <= 2; j++) {
        printf("#%d: ", j);
        for (int i = 0; i <= sz[u]; i++)
                printf("%3s%c", f[u][i][j] == INF ? "inf" : std::to_string(f[u][i][j]).c_str(), i==sz[u]?'\n':' ');
    } */
}


int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        if (readBoolean()) a.set(i);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        edgs[u].push_back(v);
        edgs[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 0; i <= n; i++) {
        ans[i] = std::min({f[1][i][0], f[1][i][1], f[1][i][2]});
        if (i >= 2) umin(ans[i], ans[i-2]);
        printf("%d ", ans[i]);
    }
    putchar('\n');
    return 0;
}