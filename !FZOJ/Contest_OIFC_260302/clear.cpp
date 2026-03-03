#include <cstdio>
#include <cstring>
#include <vector>
#include <cassert>
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch & 0xf);
    }
} using FastI::in;
struct FastO {
    char buf[1 << 20], *p3; const char *p4;
    FastO() : p3(buf), p4(buf + sizeof(buf)) {}
    inline void flush() { fwrite(buf, p3 - buf, 1, stdout), p3 = buf; }
    ~FastO() { flush(); }
    inline void operator()(char ch) {
        if (__builtin_expect(p3 == p4, 0)) fwrite(buf, sizeof buf, 1, stdout), p3 = buf;
        *p3++ = ch;
    }
    inline void operator()(long long x) {
        static long long sta[16];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) (*this)((char)(sta[sta[0]--] | '0'));
    }
} out;
constexpr int N = 600005, K = 19;
int a[N];
int st[N][K+1];
struct RMQ {
    inline int calc(int x, int y) const {
        return a[x] > a[y] ? x : y;
    }
    RMQ (int n) {
        for (int i = 1; i <= n; i++) st[i][0] = i;
        for (int k = 1; k <= K; k++)
            for (int i = 1; i + (1 << k) - 1 <= n; i++)
                st[i][k] = calc(st[i][k-1], st[i + (1<<k-1)][k-1]);
    }
    inline int operator()(int l, int r) const {
        int k = std::__lg(r - l + 1);
        return calc(st[l][k], st[r - (1<<k) + 1][k]);
    }
};
std::vector<int> edgs[N];
int fa[N][K+1];
struct FastAnc {
    FastAnc(int n) {
        for (int u = 1; u <= n; u++)
            for (int v : edgs[u])
                fa[v][0] = u;
        for (int k = 1; k <= K; k++)
            for (int i = 1; i <= n; i++)
                fa[i][k] = fa[fa[i][k-1]][k-1];
    }
    inline int operator[](int u) const { assert(u > 0); return fa[u][0]; }
    inline int operator()(int u, int mx) const {
        assert(u > 0);
        for (int k = K; k >= 0; k--)
            if (fa[u][k] && fa[u][k] <= mx) u = fa[u][k];
        return fa[u][0];
    }
};
long long bit[N];
int dfn[N], last[N];
bool indeg[N];
struct TreeSum { // 小的指向大的，最大的是根。sum返回当前节点到根的路径和
    int n, ts;
    void dfs(int u) {
        dfn[u] = ++ts;
        for (int v : edgs[u]) dfs(v);
        last[u] = ts;
    }
    TreeSum(int _n) : n(_n), ts(0) {
        for (int i = 1; i <= _n; i++)
            for (int v : edgs[i]) indeg[v] = true;
        for (int i = 1; i <= _n; i++)
            if (!indeg[i]) dfs(i);
        for (int i = 1; i <= _n; i++)
            add(dfn[i], a[i]), add(last[i] + 1, -a[i]);
    }
    inline void add(int p, int x) { for (; p <= n; p += p & -p) bit[p] += x; }
    inline long long sum(int p) { long long sum{}; for (; p > 0; p -= p & -p) sum += bit[p]; return sum; }
    inline void setTo0(int p) { add(dfn[p], -a[p]), add(last[p] + 1, a[p]); }
    inline long long operator()(int fr, int to) { return sum(dfn[fr]) - sum(dfn[to]); }
};
int tr[N];
struct BITSum {
    int n;
    BITSum(int _n) : n(_n) { for (int i = 1; i <= _n; i++) add(i, 1); }
    inline void add(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    inline int queryL(int p) { int sum{}; for (; p > 0; p -= p & -p) sum += tr[p]; return sum; }
    inline void leftAdv(int l, int r) { add(l, -1), add(r + 1, 1); }
    inline int queryR(int L) { // Returns Last R_p that Lp <= p
        int cur = 0, sum = 0;
        for (int k = K; k >= 0; k--) {
            cur |= 1 << k;
            if (cur <= n && sum + tr[cur] <= L)
                sum += tr[cur];
            else cur ^= 1 << k;
        }
        return cur;
    }
};
int mx[N];
int main() {
    freopen("clear.in", "r", stdin);
    freopen("clear.out", "w", stdout);
    int Tid, n, q;
    in(Tid), in(n), in(q);
    for (int i = 1; i <= n; i++) in(a[i]), mx[i] = i;
    /* 考虑如果**没有** Query，可以尝试连边 i-->之后第一个比a[i]大的数，每次查询树上前缀和即可
       考虑刻画每一个 Query，最后的 a_i = min_{j=i}^{R_i} a_j，那么每一个 a_j 虽然对应多个 a_i 但是只会有一次贡献。
       所以我们维护每一个 a_j 对应的极长区间中 a_x 最大的位置 记录在 mx[] 中，每个区间内只有最大值生效（其余的值全部置为0）
       操作 [l,r] 的时候，本质上是让 R_i = R_{i+1} (l<=i<r), R_r 保持不变。
        那么就可以转换为 R_r 对应的 l 区间 [X_r, Y_r] --> [X_r - 1, Y_r],
        其他的 R_i 对应的 l 区间 [X_i, Y_i] --> [X_i - 1, Y_i - 1]。
       查询 [l,r] 的时候，对于 [l, R_l] 单独做（使用 RMQ），R_l 之后的使用树上前缀和进行计算。
        注意到 [R_{i-1}+1, R_i] 的这种区间是不交的，第一次查询是 max(a[X_l..R_l])，一直跳 fa。
        之后的区间，也只会有 [R_{i-1}+1, R_i] 可能有贡献，所以只需要记录每一个 [R_{i-1}+1, R_i] 当中最大值的编号，其余的直接置为 0 即可排除多余贡献。
        最后第一个 max[l, R_l] + 后面的树上链求和。
    */
    {   static int que[N]; int hd=0, tl=0;
        for (int i = 1; i <= n; i++) {
            while ((hd^tl) && a[que[tl-1]] < a[i]) edgs[i].push_back(que[--tl]);
            que[tl++] = i;
        }
    }
    RMQ rmx(n); FastAnc fanc(n); TreeSum ftr(n); BITSum fr(n);
    for (int op, l, r; q--; ) {
        in(op), in(l), in(r);
        int Rl = fr.queryR(l), Rnxt = fr.queryR(fr.queryL(Rl+1)), Rr = fr.queryR(r);
        if (op == 1) {
            bool flg = fr.queryL(Rnxt) == fr.queryL(Rl);
            fr.leftAdv(Rl+1, Rr);
            assert(fr.queryL(Rl) <= fr.queryL(Rnxt));
            if (flg != (fr.queryL(Rnxt) == fr.queryL(Rl))) {
                if (a[mx[Rl]] < a[mx[Rnxt]]) ftr.setTo0(mx[Rl]);
                else ftr.setTo0(mx[Rnxt]), mx[Rnxt] = mx[Rl];
            }
        } else {
            int p1 = rmx(l, Rl);
            out(a[p1] + ftr(fanc[p1], fanc(p1, Rr))), out('\n');
        }
    }
    return 0;
}
