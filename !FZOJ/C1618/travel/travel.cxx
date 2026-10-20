#include <bits/stdc++.h>
using namespace std;

const int maxn = 1 << 20;
char in[maxn], out[maxn], *p1 = in, *p2 = in, *p3 = out;
#define getchar() (p1 == p2 && (p2 = (p1 = in) + fread(in, 1, maxn, stdin), p1 == p2) ? EOF : *p1++)
#define flush() (fwrite(out, 1, p3 - out, stdout))
#define putchar(x) (p3 == out + maxn && (flush(), p3 = out), *p3++ = (x))
template <typename type> void read(type &x) {
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < 48 || c > 57) {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= 48 && c <= 57) x = x * 10 + c - 48, c = getchar();
    x *= f;
}

template <typename type, typename... T> void read(type &x, T &...y) {
    read(x), read(y...);
}

template <typename type>
inline void write(type x, bool mode = 1) // 0为空格，1为换行
{
    x < 0 ? x = -x, putchar('-') : 0;
    static short Stack[50], top(0);
    do Stack[++top] = x % 10, x /= 10;
    while (x);
    while (top) putchar(Stack[top--] | 48);
    mode ? putchar('\n') : putchar(' ');
}

typedef long long ll;
const int N = 505;
int n, m, a[N], p[N];
struct Edge {
    int u, v, w, id, ma;
    bool operator<(const Edge &x) { return ma < x.ma; }
} e[N * N];
int qs;
struct Query {
    int u, v;
    ll ans;
} q[10010];
int f[N][N];
int main() {
    read(n, m);
    for (int i = 1; i <= n; i++) read(a[i]), p[i] = i;
    sort(p + 1, p + n + 1, [&](int x, int y) { return a[x] < a[y]; });
    for (int i = 1; i <= m; i++) {
        read(e[i].u, e[i].v, e[i].w);
        e[i].ma = max(a[e[i].u], a[e[i].v]);
    }
    read(qs);
    for (int i = 1; i <= qs; i++) read(q[i].u, q[i].v), q[i].ans = LLONG_MAX;
    memset(f, 0x3f, sizeof(f));
    for (int i = 1; i <= n; i++) f[i][i] = 0;
    // sort(e+1,e+m+1); int pos=1;
    for (int i = 1; i <= m; i++) f[e[i].u][e[i].v] = e[i].w;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            for (int k = 1; k <= n; k++)
                if (f[j][p[i]] + f[p[i]][k] < f[j][k]) f[j][k] = f[j][p[i]] + f[p[i]][k];
        for (int j = 1; j <= qs; j++)
            if (f[q[j].u][q[j].v] != 0x3f3f3f3f)
                if (a[p[i]] >= a[q[j].u] && a[p[i]] >= a[q[j].v])
                    q[j].ans = min(q[j].ans, f[q[j].u][q[j].v] * (ll)a[p[i]]);
    }
    for (int i = 1; i <= qs; i++)
        if (q[i].ans == LLONG_MAX)
            write(-1);
        else
            write(q[i].ans);
    flush();
    return 0;
}