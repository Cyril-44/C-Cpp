#include <stdio.h>
#include <bitset>
const int N = 2005;
bool mp[N][N], g[N][N];
int n, grp[N], size[N], revind[N]; // revind[j] = outd[j]
unsigned dfn[N], low[N], timestmp, curscc;
std::bitset<N> f[N]; // bool f[N][N] BUT WITH bitor AND count
inline void in(bool &x) {
    char ch = getchar();
    while ((ch ^ '0') && (ch ^ '1'))
        ch = getchar();
    x = ch & 1;
}
inline void umin(unsigned &x, const unsigned &y) { x = x < y ? x : y; }
void tarj(const int &u) {
    static int sta[N];
    sta[++sta[0]] = u;
    dfn[u] = low[u] = ++timestmp;
    for (int v = 0; v < n; v++)
        if (mp[u][v]) {
            if (!dfn[v]) {
                tarj(v);
                umin(low[u], low[v]);
            }
            else if (!grp[v]) umin(low[u], dfn[v]);
        }
    if (dfn[u] == low[u]) {
        ++curscc;
        do {
            grp[sta[sta[0]]] = curscc;
            ++size[curscc];
            f[curscc][sta[sta[0]]] = true;
        } while (sta[sta[0]--] ^ u);
    }
}
inline int bfs() {
    static int q[N*N];
    int l = 0, r = 0, ans = 0;
    for (int i = 1; i <= curscc; i++)
        if (!revind[i]) q[r++] = i;
    if (!r) q[r++] = 0;
    for (; l < r; ++l) {
        for (int i = 1; i <= curscc; i++)
            if (g[q[l]][i]) {
                f[i] |= f[q[l]], --revind[i];
                if (!revind[i]) q[r++] = i;
            }
    }
    for (int i = 1; i <= curscc; i++)
        ans += size[i] * f[i].count();
    return ans;
}
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            in(mp[i][j]);
    for (int i = 0; i < n; i++)
        if (!dfn[i]) tarj(i);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (mp[i][j] && (grp[i] ^ grp[j]))
                g[grp[j]][grp[i]] = true, ++revind[grp[i]];
    printf("%d\n", bfs());
    return 0;
}
