#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <random>
#include <algorithm>
#include <map>
#include <stack>
constexpr int N = 1005;
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FastI() : p1(), p2() {}
    [[gnu::always_inline]] inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    template<typename T>
    inline void operator()(T &x) {
        char ch = gc();
        bool flg{};
        while ((ch < '0' || ch > '9') && (ch != '-')) ch = gc();
        if (ch == '-') flg = true, ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
        if (flg) x = -x;
    }
} in;
struct Point {
    int x, y, v;
    inline bool operator<(const Point &o) const { return x + y < o.x + o.y; }
} pnts[N << 1];
/*
考虑只有 Pos_b < Pos_c 或者 Pos_d < Pos_a 的时候 p 才会有负贡献。
设置 F[i][j] 表示当前已经选了前 i 个红色和前 j 个紫色的位置，产生的最小负贡献。
经典 trick：考虑将这个状态用平面直角坐标系去刻画，发现实质上我们要弄一条 (0,0) 到 (n,m) 的路径。
发现 (b,c) 对应的点如果有贡献一定在最终路径的上方，(a,d) 则是下方。
    有题目保证 a<b 且 c<d，我们知道 (b,c) 在 (a,d) 的左上方，不存在选了 (b,c) 没选 (a,d) 的情况。
    所以我们直接设置 (a,d) 权值为 p，(b,c) 权值为 -p。
换一种方式去理解这个 DP，考虑 j 是高度，那么有 F[i][j] = min(F[i-1][1]...F[i-1][j]) + Sum[i][j]，其中 Sum[i][j] 是 sum_{k=1}^j A[i][k]。
那么我们只需要每个转移做一个 Prefix-Min，然后去加上前缀和即可。
直接暴力 Prefix-Min，暴力加即可。
可以使用线段树二分去模拟 Treap/Splay 的 lower_bound 分裂。
*/

struct Splay {
#define Lc ch[0]
#define Rc ch[1]
#define DOCHILD for (int t = 0; t ^ 2; t++)
    constexpr static int64_t COV_DFT = (1ll << 62) - 1;
    struct Node {
        int fa, ch[2], cnt;
        int64_t val, rg[2], add, cov;
        inline void pull(int64_t p, int64_t c) {
            if (c != COV_DFT) val = rg[0] = rg[1] = cov = c, add = 0;
            if (p) val += p, rg[0] += p, rg[1] += p, add += p;
        }
    } tr[N];
    int totcnt, rt;
    Splay() : totcnt(), rt() {}
    inline void pushup(int u) {
        DOCHILD { tr[u].rg[t] = tr[u].ch[t] ? tr[tr[u].ch[t]].rg[t] : tr[u].val; }
        tr[u].cnt = tr[tr[u].Lc].cnt + 1 + tr[tr[u].Rc].cnt;
    }
    inline void pushdown(int u) {
        if (!tr[u].add && tr[u].cov == COV_DFT) return;
        DOCHILD { if (tr[u].ch[t]) tr[tr[u].ch[t]].pull(tr[u].add, tr[u].cov); }
        tr[u].add = 0, tr[u].cov = COV_DFT;
    }
    inline int alloc(int fa, int ls=0, int rs=0) {
        tr[++totcnt] = {fa, {ls, rs}, 0, 0ll, {0ll,0ll}, 0ll,COV_DFT};
        pushup(totcnt);
        return totcnt;
    }
    inline void sync(int u) {
        std::stack<int> sta;
        for (; u; u = tr[u].fa) sta.push(u);
        for (; !sta.empty(); sta.pop()) pushdown(sta.top());
    }
    inline bool type(int u) { return tr[tr[u].fa].Rc == u; }
    inline void rotate(int u) {
        bool t = type(u); int f = tr[u].fa;
        bool tf = type(f); int anc = tr[f].fa;
        int &chnt = tr[u].ch[!t];
        tr[f].ch[t] = chnt; if (chnt) tr[chnt].fa = f; // u.fa ---[t]--> u.!t
        chnt = f, tr[f].fa = u;                        // u ---[!t]--> u.fa
        if (anc) tr[anc].ch[tf] = u, tr[u].fa = anc;   // u.fa.fa ---[tf]--> u
        pushup(f);
        pushup(u);
    }
    inline void splay(int u, int top = 0) {
        sync(u);
        while (tr[u].fa != top) {
            if (tr[tr[u].fa].fa != top)
                rotate(type(u) == type(tr[u].fa) ? tr[u].fa : u);
            rotate(u);
        }
        if (!top) rt = u;
    }
    inline void build(int num) {
        rt = alloc(0);
        for (; num--; rt = tr[rt].Rc) tr[rt].Rc = alloc(rt);
        splay(rt);
    }
    inline int lower_bound(int u, int64_t w) {
        sync(u);
        while (u) {
            pushdown(u);
            if (tr[u].Lc && tr[tr[u].Lc].rg[1] <= w) u = tr[u].Lc;
            if (tr[u].val <= w) return u;
            u = tr[u].Lc;
        }
        return 0; // reach end
    }
    inline void operate(bool type, bool child, int u, int64_t val) { // Operate u->ch, 0:add, 1:cov
        int64_t op[2]{0, COV_DFT};
        op[type] = val;
        if (tr[u].ch[child]) tr[tr[u].ch[child]].pull(op[0], op[1]);
    }
    inline int64_t getval() { return tr[rt].val; }
} f;

std::map<int,int64_t> v[N];
namespace BF {
    constexpr int N = 5;
    int64_t f[N][N], s[N][N];
    inline int64_t work(int n, int m) {
        for (int i = 1; i <= n; i++) {
            int64_t premn = 0;
            for (int j = 1; j <= m; j++) {
                s[i][j] = s[i][j-1] + v[i][j];
                f[i][j] = (premn = std::min(premn, f[i-1][j])) + s[i][j];
            }
        }
        /* for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                printf("%ld%c", f[i][j], " \n"[j==m]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                printf("%ld%c", s[i][j], " \n"[j==m]); */
        return f[n][m];
    }
}
int main(int argc, char** argv) {
    int n, m, k;
    in(n), in(m), in(k);
    int64_t sum = 0;
    for (int i = 1; i <= k; i++) {
        int a, b, c, d, p;
        in(a), in(b), in(c), in(d), in(p);
        v[a][d] += p;
        v[b][c] -= p;
        // printf("(%d,%d)+=%d\n(%d,%d)-=%d\n", a,d,p,b,c,p);
        sum += p;
    }
    if (false && argc > 1 && !strcmp(argv[1], "dbg")) return (n<=2000&&printf("%ld\n", - BF::work(n, m))), 0;
    f.build(m + 1);
    // f.debug(f.rt); putchar('\n');
    int64_t ans;
    for (int i = 1; i <= n; i++) {
        std::vector<int> rts; rts.reserve(v[i].size());
        for (auto j : v[i]) {
            // printf("([%d], %ld, sp%d)", j.first, j.second, j.first - lastPos);
            f.splay(j.first); // with a guard
            f.operate(0, 1, f.rt, j.second); // Right += j.second
            rts.push_back(j.first + 1);
            // f.debug(ret.first); printf("| ");
        }
        // f.debug(rts.back()); putchar('\n');
        for (int i = 1; i < (int)v[i].size(); i++) {
            f.splay(rts[i-1]);
            f.splay(rts[i]+1, rts[i-1]);
            int u = f.lower_bound(f.tr[rts[i]+1].Lc, f.getval());
            if (u) f.splay(u, rts[i]+1);
            else u = rts[i]+1;
            f.operate(1, 0, u, f.getval()); // Left += premn
        }
        
        // f.debug(f.rt); putchar('\n');
    }
    printf("%ld\n", -f.getval());
    return 0;
}
