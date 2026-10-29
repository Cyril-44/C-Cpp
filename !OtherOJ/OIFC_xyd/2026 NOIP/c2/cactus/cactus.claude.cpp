#include <bits/stdc++.h>
using namespace std;

constexpr int N = 20, B = 20;
constexpr unsigned MASK = (1u << B) - 1;

unsigned adj[N + 5];
static unsigned f[1 << N][N];   // ((v+1) << B) | T，0 表示无效，约 80MB
static int g[1 << N];
int n;

// v 大的覆盖，v 相同则起点集合取并
static inline void upd(unsigned &o, unsigned x) {
    unsigned a = x >> B, b = o >> B;
    if (a > b) o = x;
    else if (a == b) o |= x;
}

int main() {
    int C, T, m;
    scanf("%d%d", &C, &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        memset(adj, 0, sizeof adj);
        for (int e = 0, x, y; e < m; e++) {      // 不能写 m--，后面答案要用原来的 m
            scanf("%d%d", &x, &y);
            --x, --y;
            adj[x] |= 1u << y;
            adj[y] |= 1u << x;
        }

        // 判连通：位集扩张
        unsigned vis = 1, full = (1u << n) - 1;
        for (bool ch = true; ch;) {
            ch = false;
            for (int i = 0; i < n; i++)
                if ((vis >> i & 1) && (adj[i] & ~vis)) vis |= adj[i], ch = true;
        }
        if (vis != full) { puts("-1"); continue; }

        // 只清当前 n 用到的部分
        memset(f, 0, sizeof(unsigned) * N * (size_t)(1 << n));
        memset(g, 0, sizeof(int) * (size_t)(1 << n));

        for (unsigned S = 0; S <= full; S++) {
            int gs = g[S];

            // (1) 加孤立点；(2) 起一条新路径，j 是第一个新点 a1，此时不闭合
            for (int j = 0; j < n; j++) if (!(S >> j & 1)) {
                unsigned S2 = S | 1u << j;
                if (g[S2] < gs) g[S2] = gs;
                if (unsigned t = adj[j] & S)
                    upd(f[S2][j], ((unsigned)(gs + 1) << B) | t); // 注意这里的 +1 只是偏移，不是路径长度 +1
            }

            // (3) 从 f 出发：延伸 + 闭合
            for (int i = 0; i < n; i++) {
                unsigned x = f[S][i];
                if (!x) continue;
                int v = (int)(x >> B) - 1;
                unsigned Tset = x & MASK;
                for (unsigned msk = adj[i] & ~S; msk; msk &= msk - 1) {
                    int j = __builtin_ctz(msk);
                    unsigned S2 = S | 1u << j;
                    upd(f[S2][j], x);                          // 延伸：j 是中间点
                    if ((Tset & adj[j]) && g[S2] < v + 1)      // 闭合：j 连回某个 u，环数 +1
                        g[S2] = v + 1;
                }
            }
        }
        printf("%d\n", m - (n - 1 + g[full]));
    }
}