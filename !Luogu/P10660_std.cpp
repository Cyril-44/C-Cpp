#include <bits/stdc++.h>
using namespace std;
inline void rd() {}
template<typename T, typename ...U>
inline void rd(T &x, U &...args) {
    char ch = getchar();
    T f = 1;
    x = 0;

    while (ch < '0' || ch > '9') {
        if (ch == '-')
            f = -1;

        ch = getchar();
    }

    while (ch >= '0' && ch <= '9')
        x = (x << 1) + (x << 3) + (ch ^ 48), ch = getchar();

    x *= f;
    rd(args...);
}
const int N = 3e4 + 5, mod = 10007;
struct Matrix {
    int m[2][2];
    Matrix() {
        memset(m, 0, sizeof m);
    }
    Matrix(int _x) {
        memset(m, 0, sizeof m);
        m[0][0] = m[1][1] = _x;
    }
    Matrix friend operator*(Matrix a, Matrix b) {
        Matrix c;

        for (int i = 0; i <= 1; i++)
            for (int j = 0; j <= 1; j++)
                for (int k = 0; k <= 1; k++)
                    (c.m[i][j] += a.m[i][k] * b.m[k][j]) %= mod;

        return c;
    }
};
inline int KSM(int x, int n) {
    int ans = 1;

    while (n) {
        if (n & 1)
            ans = ans * x % mod;

        x = x * x % mod;
        n >>= 1;
    }

    return ans;
}
int n, q;
namespace LCT {
int f[N], ch[N][2], tag[N];
Matrix m[N], prd[N];
struct {
    int x, y, ans;
} nd[N];
#define ls ch[p][0]
#define rs ch[p][1]
inline void PushUp(int p) {
    prd[p] = prd[ls] * m[p] * prd[rs];
}
inline void Rev(int p) {
    tag[p] ^= 1;
    swap(ls, rs);
    PushUp(p);
}
inline void PushDown(int p) {
    if (!tag[p])
        return ;

    Rev(ls), Rev(rs);
    tag[p] = 0;
}
inline int Get(int p) {
    return ch[f[p]][1] == p;
}
inline int IsRoot(int p) {
    return ch[f[p]][1] != p && ch[f[p]][0] != p;
}
void Update(int p) {
    if (!IsRoot(p))
        Update(f[p]);

    PushDown(p);
}
inline void Rotate(int u) {
    int fa = f[u], anc = f[fa], tp = Get(u);

    if (!IsRoot(fa))
        ch[anc][Get(fa)] = u;

    ch[fa][tp] = ch[u][!tp], ch[u][!tp] = fa;
    f[fa] = u, f[u] = anc, f[ch[fa][tp]] = fa;
    PushUp(fa), PushUp(u);
}
inline void Splay(int x) {
    Update(x);

    for (int fa = f[x]; !IsRoot(x); Rotate(x), fa = f[x])
        if (!IsRoot(fa))
            Rotate(Get(fa) == Get(x) ? fa : x);
}
inline int Access(int x) {
    int p;

    for (p = 0; x; p = x, x = f[x])
        Splay(x), ch[x][1] = p, PushUp(x);

    return p;
}
inline void MakeRoot(int x) {
    Access(x);
    Splay(x);
    Rev(x);
}
inline int FindRoot(int p) {
    p = Access(p);

    while (ls)
        p = ls, PushDown(p);

    return Splay(p), p;
}
inline void Link(int x, int y) {
    if (FindRoot(x) == FindRoot(y)) {
        MakeRoot(x);
        Access(y);
        Matrix mm = prd[ch[x][1]] * m[x];
        nd[x].x = x, nd[x].y = y;
        int k = (mm.m[0][0] + mod - 1) % mod, b = mod - mm.m[1][0];

        if (k == 0 && b == 0)
            nd[x].ans = -2;
        else if (k == 0)
            nd[x].ans = -1;
        else
            nd[x].ans = 1ll * b * KSM(k, mod - 2) % mod;
    } else {
        MakeRoot(x);
        f[x] = y;
    }
}
inline void Modify(int x, int k, int y, int b) {
    int rt = FindRoot(x);
    Splay(x);
    m[x].m[0][0] = k, m[x].m[1][0] = b;
    PushUp(x);

    if (rt != x) {
        Access(x);
        Splay(x);
        f[ch[x][0]] = 0, ch[x][0] = 0;
        PushUp(x);
        Link(x, y);
        Link(nd[rt].x, nd[rt].y);
    } else
        Link(x, y);
}
inline int Query(int p) {
    int rt = FindRoot(p);
    Access(p);
    Splay(rt);
    int k = prd[ch[rt][1]].m[0][0], b = prd[ch[rt][1]].m[1][0];

    if (nd[rt].ans == -1)
        return -1;

    if (nd[rt].ans == -2)
        return -2;
    fprintf(stderr, "Que %d: %d * \033[4m%d\033[0m + %d\n", p, k, nd[rt].ans, b);
    return (1ll * k * nd[rt].ans % mod + b) % mod;
}
void print(int u, int dep = 0) {
    if (!u) return;
    PushDown(u);
    print(ch[u][0], dep + 2);
    for (int i = dep; i--; putchar(' '));
    PushUp(u);
    printf("%d: %d %d\n", u, prd[u].m[0][0], prd[u].m[1][0]);
    print(ch[u][1], dep + 2);
}
#undef ls
#undef rs
}
int p[N];
using namespace LCT;
signed main() {
    rd(n);
    prd[0] = m[0] = Matrix(1);

    for (int i = 1; i <= n; i++) {
        int k, b;
        rd(k, p[i], b);
        m[i].m[0][0] = k;
        m[i].m[1][0] = b;
        m[i].m[1][1] = 1;
    }

    for (int i = 1; i <= n; i++)
        Link(i, p[i]);
    print(5);
    rd(q);

    while (q--) {
        char s[3];
        int a, x, y, z;
        scanf("%s", s);

        if (s[0] == 'A') {
            rd(a);
            printf("%d\n", Query(a));
            print(5);
        } else if (s[0] == 'C') {
            rd(a, x, y, z);
            Modify(a, x, y, z);
        }
    }

    return 0;
}
