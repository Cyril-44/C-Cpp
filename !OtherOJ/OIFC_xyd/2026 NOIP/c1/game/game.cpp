#include <bits/stdc++.h>
constexpr int N = 200005, MOD = (int)1e9 + 7;

struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    int get() const { return val; }
    Mint(int v = 0) : val(v) {}
private: int val;
};

std::vector<int> g[N];
int par[N], len[N], lson[N], dfn[N], ord[N], n;

/*
 * 线段树：区间仿射变换 x -> m*x + a，单点查询。
 * 叶子初始值为 1。位置 = dfn（同一条长链的 dfn 连续）。
 * 一个点 u 在位置 dfn[u]+i 上存的是 C_u(i)：
 *   以 u 为根、高度 <= i 的连通子树个数（i 同时也是“绝对深度”的偏移，所以重儿子的数组不用平移）。
 */
Mint tm_[N << 2], ta_[N << 2];
inline void app(int o, Mint m, Mint a) { tm_[o] *= m; ta_[o] = ta_[o] * m + a; }
inline void push(int o) {
    app(o << 1, tm_[o], ta_[o]); app(o << 1 | 1, tm_[o], ta_[o]);
    tm_[o] = 1; ta_[o] = 0;
}
void upd(int o, int l, int r, int ql, int qr, Mint m, Mint a) {
    if (ql <= l && r <= qr) { app(o, m, a); return; }
    push(o);
    int mid = (l + r) >> 1;
    if (ql <= mid) upd(o << 1, l, mid, ql, qr, m, a);
    if (qr > mid) upd(o << 1 | 1, mid + 1, r, ql, qr, m, a);
}
Mint qry(int o, int l, int r, int p) {
    if (l == r) return tm_[o] + ta_[o];          // 叶子初值为 1：tm*1 + ta
    push(o);
    int mid = (l + r) >> 1;
    return p <= mid ? qry(o << 1, l, mid, p) : qry(o << 1 | 1, mid + 1, r, p);
}
void build() { for (int i = 0; i < (N << 2); i++) tm_[i] = 1, ta_[i] = 0; }

int main() {
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    // ---- 非递归 dfs1：求 par / len / lson（防止链形数据爆栈）
    {
        std::vector<int> q = {1};
        par[1] = 0;
        for (size_t h = 0; h < q.size(); h++) {
            int u = q[h];
            for (int v : g[u]) if (v != par[u]) par[v] = u, q.push_back(v);
        }
        for (int k = n - 1; k >= 1; k--) {
            int u = q[k], p = par[u];
            if (len[u] >= len[p]) len[p] = len[u] + 1, lson[p] = u;
        }
    }
    // ---- 非递归 dfs2：重儿子紧跟在父亲后面，使得一条长链的 dfn 连续
    {
        std::vector<int> stk = {1};
        int cnt = 0;
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            dfn[u] = ++cnt, ord[cnt] = u;
            for (int v : g[u]) if (v != par[u] && v != lson[u]) stk.push_back(v);
            if (lson[u]) stk.push_back(lson[u]);
        }
    }
    build();
    // ---- 按 dfn 从大到小处理（子孙先于祖先）。根(dfn=1)不处理，保留它各个儿子的数组。
    // C_u(i) = prod_{child c} (1 + C_c(i-1)),  C_u(0) = 1
    for (int idx = n; idx >= 2; idx--) {
        int u = ord[idx];
        if (!lson[u]) continue;                                   // 叶子：只有 C(0)=1
        // 重儿子：整段 (1 + x)
        upd(1, 1, n, dfn[u] + 1, dfn[u] + len[u], 1, 1);
        // 轻儿子 v：v 的第 i 项对应 u 的第 i+1 项
        for (int v : g[u]) if (v != par[u] && v != lson[u]) {
            Mint tot = 0;
            for (int i = 0; i <= len[v]; i++) {
                Mint c = qry(1, 1, n, dfn[v] + i) + 1;            // 1 + C_v(i)
                upd(1, 1, n, dfn[u] + 1 + i, dfn[u] + 1 + i, c, 0);
                tot = c;
            }
            // i > len[v] 之后 C_v 饱和为总数：后缀乘常数 (1 + T_v)
            if (len[v] + 2 <= len[u])
                upd(1, 1, n, dfn[u] + len[v] + 2, dfn[u] + len[u], tot, 0);
        }
    }
    // ---- 取出根的各个儿子的最终数组
    static Mint C[N];
    for (int v : g[1])
        for (int i = 0; i <= len[v]; i++) C[dfn[v] + i] = qry(1, 1, n, dfn[v] + i);

    // ---- 统计：1 是直径中点  <=>  最大深度恰为 d，且至少有两个儿子子树达到深度 d
    // 外加只含顶点 1 的子图（Alice 无路可走，Bob 赢）
    Mint ans = 1, dead = 1;              // dead = 已饱和（不可能再恰好取到当前层）的儿子的 (1+T_v) 之积
    std::vector<int> vs(g[1].begin(), g[1].end());
    for (int i = 0; i < len[1]; i++) {   // 儿子子树高度恰为 i（深度 i+1）
        std::vector<int> nvs;
        for (int v : vs) {
            if (len[v] < i) dead *= C[dfn[v] + len[v]] + 1;
            else nvs.push_back(v);
        }
        vs.swap(nvs);
        Mint x0 = dead, x1 = 0, x2 = 0;  // 0 个 / 恰好 1 个 / 至少 2 个儿子恰好取到高度 i
        for (int v : vs) {
            Mint cur = C[dfn[v] + i];
            Mint prev = i ? C[dfn[v] + i - 1] : Mint(0);
            Mint A = prev + 1;           // 不选 v，或高度 <= i-1
            Mint B = cur - prev;         // 高度恰为 i
            x2 = x2 * (A + B) + x1 * B;
            x1 = x1 * A + x0 * B;
            x0 = x0 * A;
        }
        ans += x2;
    }
    printf("%d\n", ans.get());
}