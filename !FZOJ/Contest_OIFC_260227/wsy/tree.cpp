#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <vector>
#include <set>
#include <cstring>
#include <unordered_map>
#include <bitset>
#include <cassert>
#include <map>

using namespace std;

const int maxn = 1 << 20;
char in[maxn],out[maxn],*p1=in,*p2=in,*p3=out;
#define getchar() (p1==p2&&(p2=(p1=in)+fread(in,1,maxn,stdin),p1==p2)?EOF:*p1++)
#define flush() (fwrite(out,1,p3-out,stdout))
#define putchar(x) (p3==out+maxn&&(flush(),p3=out),*p3++=(x))
template<typename type>
void read(type &x)
{
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < 48 || c > 57) 
    {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= 48 && c <= 57)
        x = x * 10 + c - 48, c = getchar();
    x *= f;
}

template<typename type, typename ...T>
void read(type &x, T &...y)
{
    read(x), read(y...);
}

template<typename type>
inline void write(type x,bool mode=1)//0为空格，1为换行
{
    x < 0 ? x = -x, putchar('-') : 0;
    static short Stack[50], top(0);
    do Stack[++top]=x%10,x/=10; while(x);
    while(top) putchar(Stack[top--]|48);
    mode?putchar('\n'):putchar(' ');
}

#define ll long long

const int N = 100005;
const int mod = 1000000007;
const int INF = 0x3f3f3f3f;

int n, m;
int g[205][205], a[N], f[205], rk[205], dep[N], dfn[N], tc;
vector<int> e[N];
int dp[N][32], h[32];

void dfs2(int u, int fa) {
    memset(dp[u], 0x3f, sizeof(dp[u]));
    dp[u][(1 << a[u])] = 1;
    for (int v : e[u]) if (v != fa) {
        dfs2(v, u);
        memcpy(h, dp[u], sizeof(h));
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                h[i | j] = min(h[i | j], dp[u][i] + dp[v][j]);
            }
        }
        memcpy(dp[u], h, sizeof(dp[u]));
    }
}

void work1() {
    for (int i = 1; i <= n; i++) {
        a[i]--;
    }
    dfs2(1, 0);
    int ans = 0x3f3f3f3f;
    for (int i = 1; i <= n; i++) {
        ans = min(ans, dp[i][31]);
    }
    write(ans);
}

void dfs(int u, int fa) {
    f[u] = fa;
    dfn[u] = ++tc;
    dep[u] = dep[fa] + 1;
    rk[tc] = u;
    for (int v : e[u]) if (v != fa) {
        dfs(v, u);
    }
}

int get(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    while (dep[x] > dep[y]) {
        x = f[x];
    }
    while (x != y) {
        x = f[x], y = f[y];
    }
    return x;
}

void work2() {
    dfs(1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            g[i][j] = get(i, j);
        }
    }
    int vec[N], tot = 0;
    for (int i = 1; i <= n; i++) {
        vec[++tot] = i;
    }
    sort(vec + 1, vec + 1 + n, [](int a, int b) {
        return dfn[a] < dfn[b];
    });
    int res = n;
    for (int ii = 1; ii <= n; ii++) {
        int i = vec[ii];
        for (int jj = ii + 1; jj <= n; jj++) {
            int j = vec[jj];
            if (a[i] == a[j]) continue;
            for (int kk = jj + 1; kk <= n; kk++) {
                int k = vec[kk];
                if (a[k] == a[i] || a[k] == a[j]) continue;
                int ans = dep[i] + dep[j] + dep[k] - dep[g[i][j]] - dep[g[j][k]] - dep[g[g[i][j]][k]] + 1;
                if (ans >= res) continue;
                for (int xx = kk + 1; xx <= n; xx++) {
                    int x = vec[xx];
                    if (a[x] == a[i] || a[x] == a[j] || a[x] == a[k]) continue;
                    int ans = dep[i] + dep[j] + dep[k] + dep[x] - dep[g[i][j]] - dep[g[j][k]] - dep[g[k][x]] - dep[g[g[g[i][x]][j]][k]] + 1;
                    if (ans >= res) continue;
                    for (int yy = xx + 1; yy <= n; yy++) {
                        int y = vec[yy];
                        if (a[y] == a[x] || a[y] == a[i] || a[y] == a[j] || a[y] == a[k]) continue;
                        int t = g[g[g[g[i][j]][k]][x]][y];
                        int ans = dep[i] + dep[j] + dep[k] + dep[x] + dep[y] - dep[g[i][j]] - dep[g[j][k]] - dep[g[k][x]] - dep[g[x][y]] - dep[t] + 1;
                        res = min(res, ans);
                    }
                }
            }
        }
    }
    write(res);
}

int main() {
    // freopen("tree.in", "r", stdin);
    // freopen("tree.out", "w", stdout);
    read(n);
    int mx = 0;
    for (int i = 1; i <= n; i++) {
        read(a[i]);
        mx = max(mx, a[i]);
    }
    for (int i = 1; i < n; i++) {
        int a, b;
        read(a, b);
        e[a].push_back(b);
        e[b].push_back(a);
    }

    if (mx <= 5) work1();
    else work2();

    // else work3();

    flush();
    return 0;
}