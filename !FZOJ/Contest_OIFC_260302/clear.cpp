#include <cstdio>
#include <cstring>
#include <vector>
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        signed char flg = 1;
        while ((ch < '0' || ch > '9') && ch != '-') ch = gc();
        if (ch == '-') ch = gc(), flg = -1;
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch & 0xf);
        x *= flg;
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
    inline void operator()(int x) {
        if (x < 0) (*this)('-'), x = -x;
        static int sta[16];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) (*this)((char)(sta[sta[0]--] | '0'));
    }
} out;
constexpr int N = 600005, K = 20;
int a[N];
struct RMQ {
    static int st[N][K+1];
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
struct FastAnc {
    static int fa[N][K];
    FastAnc(int n) {
        for (int u = 1; u <= n; u++)
            for (int v : edgs[u])
                fa[v][0] = u;
        for (int k = 1; k <= K; k++)
            for (int i = 1; i <= n; i++)
                fa[i][k] = fa[fa[i][k-1]][k-1];
    }
    inline int operator()(int u, int mx) const {
        for (int k = K; k >= 0; k--)
            if (fa[u][k] && fa[u][k] <= mx) u = fa[u][k];
        return fa[u][0];
    }
};
struct FenwickTree { // 小的指向大的，最大的是根。sum返回当前节点到根的路径和
    int n, ts;
    static long long bit[N];
    static int dfn[N], last[N];
    static bool indeg[N];
    void dfs(int u) {
        dfn[u] = ++ts;
        for (int v : edgs[u]) dfs(v);
        last[u] = ts;
    }
    FenwickTree(int _n) : n(_n), ts(0) {
        for (int i = 1; i <= _n; i++)
            for (int v : edgs[i]) indeg[v] = true;
        for (int i = 1; i <= _n; i++)
            if (!indeg[i]) dfs(i);
    }
    inline void add(int p, int x) { for (; p <= n; p += p & -p) bit[p] += x; }
    inline long long sum(int p) { int sum{}; for (; p > 0; p -= p & -p) sum += bit[p]; return sum; }
    inline void setTo0(int p) { add(dfn[p], a[p]), add(last[p] + 1, -a[p]); }
    inline int operator()(int fr, int to) { return sum(dfn[fr]) - sum(dfn[to]); }
};
int mx[N];
int main() {
    freopen("clear.in", "r", stdin);
    freopen("clear.out", "w", stdout);
    int Tid, n, q;
    in(Tid), in(n), in(q);
    for (int i = 1; i <= n; i++) in(a[i]);
    RMQ rmx(n);
    /* 考虑如果**没有** Query，可以尝试连边 i-->之后第一个比a[i]大的数，每次查询树上前缀和即可
       考虑刻画每一个 Query，最后的 a_i = min_{j=i}^{R_i} a_j，那么每一个 a_j 虽然对应多个 a_i 但是只会有一次贡献。
       所以我们维护每一个 a_j 对应的极长区间中 a_x 最大的位置 记录在 mx[] 中，每个区间内只有最大值生效（其余的值全部置为0）
       操作 [l,r] 的时候，本质上是让 R_i = R_{i+1} (l<=i<r), R_r 保持不变。
        那么就可以转换为 R_r 对应的 l 区间 [X_r, Y_r] --> [X_r - 1, Y_r],
        其他的 R_i 对应的 l 区间 [X_i, Y_i] --> [X_i - 1, Y_i - 1]。
       查询 [l,r] 的时候，对于 [l, R_l] 单独做（使用 RMQ），R_l 之后的使用树上前缀和进行计算。
        注意到 [Y_i+1, R_i] 的这种区间是不交的，第一次查询是 max(a[X_l..R_l])，一直跳 fa。
        之后的区间，也只会有 [Y_i+1, R_i] 可能有贡献，所以只需要记录每一个 [Y_i+1, R_i] 当中最大值的编号，其余的直接置为 0 即可排除多余贡献。
        最后第一个 max[l, R_l] + 后面的树上链求和。
    */
    {   static int que[N]; int hd=0, tl=0;
        for (int i = 1; i <= n; i++) {
            while ((hd^tl) && a[que[tl-1]] < a[i]) edgs[i].push_back(que[--tl]);
            que[tl++] = i;
        }
    }
    FastAnc fanc(n); FenwickTree fw(n);
    for (int op, l, r; q--; ) {
        in(op), in(l), in(r);

    }
    return 0;
}
