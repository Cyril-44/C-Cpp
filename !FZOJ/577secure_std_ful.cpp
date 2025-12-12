#include <bits/stdc++.h>
using namespace std;

/*
  注：下面对原代码逐行/逐段添加中文注释，解释变量含义、数据结构与算法流程。
  该程序核心是：树上重链剖分（HLD）+每条链单独线段树 + 每个点的可删除优先队列（延迟删除）
  用于维护若干“带半径的点”（位置 P 和半径 K），支持插入/删除与路径可覆盖性查询。
*/

#define pri pair<int, int>
#define mkp make_pair

// 快速整数输入（手写，略微不安全但常见于竞赛模板）
template <typename T> void in(T &n) {
    n = 0;
    char c = getchar();
    bool flag = 0;
    for (; c < '0' || c > '9'; c = getchar())
        if (c == '-') flag = 1;
    for (; c >= '0' && c <= '9'; c = getchar()) (n *= 10) += (c ^ 48);
    if (flag) n = -n;
}

int wlsk[45];
int wltp;
// 快速整数输出（使用全局缓冲数组 wlsk/wltp）
template <typename T> void out(T n, char c = 0) {
    if (n == 0) {
        putchar('0');
        if (c) putchar(c);
        return;
    }
    if (n < 0) putchar('-'), n = -n;
    while (n) wlsk[++wltp] = (n % 10), n /= 10;
    while (wltp) putchar(wlsk[wltp--] ^ 48);
    if (c) putchar(c);
}
// 取最大值的辅助函数
void chkmx(int &x, int y) {
    x = max(x, y);
}

const int N = 5e5 + 5;   // 最大节点数（树节点上限）
const int M = 2e6 + 5;   // 线段树/节点池上限（根据链树构建规模）
const int inf = 1e9 + 7; // 近似无穷大

vector<pri> e[N]; // 邻接表：e[u] 存 (v, w) 表示 u-v 边权为 w

// 树的重链剖分相关数组
int fa[N];    // 父
int d[N];     // 深度
int top[N];   // 链顶
int s[N];     // 从根到该点的距离前缀和
int ss[N];    // 按 dfn 排序的 s 值
int bot[N];   // 链底节点
int sz[N];    // 子树大小
int hs[N];    // 重儿
int dfn[N];   // 节点到序号
int dfns[N];  // 序号到节点
int tdfn = 0; // 当前序号

// dfs1:计算父、深度、子树大小、重儿、前缀距离 s
void dfs1(int u, int Fa, int D, int S) {
    fa[u] = Fa;
    d[u] = D;
    sz[u] = 1;
    hs[u] = 0;
    s[u] = S; // s[u] 表示从根到 u 的距离（边权和）
    for (auto each : e[u]) {
        auto v = each.first;
        auto w = each.second;
        if (v == Fa) continue;
        dfs1(v, u, D + 1, S + w);
        sz[u] += sz[v];
        if (sz[v] > sz[hs[u]]) hs[u] = v; // 记录重儿（子树最大）
    }
}

// dfs2:重链剖分第二步，设置 top、dfn、bot（链底）
void dfs2(int u, int tp) {
    top[u] = tp;
    dfn[u] = ++tdfn;
    dfns[tdfn] = bot[u] = u;                         // 初始链底为自身
    if (hs[u]) dfs2(hs[u], tp), bot[u] = bot[hs[u]]; // 先处理重儿，链底继承重儿的链底
    for (auto each : e[u]) {
        auto v = each.first;
        auto w = each.second;
        if (v != fa[u] && v != hs[u]) dfs2(v, v); // 轻儿各自成为新链
    }
}

/*
  可删除优先队列（延迟删除）
  用两个 priority_queue 实现：q1 存实际插入，q2 存要删除的值；
  top() 时同步弹出相等的元素，返回当前最大值或 -inf 表示空。
*/
struct mset {
    priority_queue<int> q1, q2;
    void ins(int x) { q1.push(x); }
    void del(int x) { q2.push(x); }
    int top() {
        while (!q2.empty()) {
            if (q1.top() != q2.top()) break;
            q1.pop();
            q2.pop();
        }
        return q1.empty() ? -inf : q1.top();
    }
} ms[N]; // 每个树节点维护一个 mset，用于存放落在该节点的 K 值（半径或权值）

int root[N]; // 每个链的线段树根（以链顶索引为 key 存放对应链的线段树根）

// 线段树全局数组（命名空间 sgt 中使用）
#define ls son[o][0]
#define rs son[o][1]
#define mid ((l + r) >> 1)

namespace sgt {
int son[M][2], L[M], R[M];
int mx[M][2]; //  mx[o][0] = max(ms[u].top() + s[u]) 在区间上的最大值
              // mx[o][1] = max(ms[u].top() - s[u]) 在区间上的最大值（用于另一类查询）
int cc[M];    // cc[o] 用作区间覆盖计数（正负增减）
int totn = 0;
bool cov[M]; // cov[o] 表示该区间是否被覆盖（布尔）

// pushupc: 更新 cov[o]（是否被覆盖）
void pushupc(int o) {
    cov[o] = cov[ls] && cov[rs];
    cov[o] = (cov[o]) || bool(cc[o]); // 如果当前节点计数非零也视为覆盖
}

// pushupmx: 合并左右子树的 mx 值
void pushupmx(int o) {
    mx[o][0] = max(mx[ls][0], mx[rs][0]);
    mx[o][1] = max(mx[ls][1], mx[rs][1]);
}

// build: 为区间 [l,r] 建线段树节点，叶子节点根据 dfns[l]（序号->节点）初始化 mx
int build(int l, int r) {
    int o = ++totn;
    L[o] = l;
    R[o] = r;
    cc[o] = 0;
    cov[o] = 0;
    if (l == r) {
        int u = dfns[l];               // 序号 l 对应的树节点 u
        mx[o][0] = ms[u].top() + s[u]; // ms[u].top() 可能为 -inf
        mx[o][1] = ms[u].top() - s[u];
        return o;
    }
    ls = build(l, mid);
    rs = build(mid + 1, r);
    pushupc(o);
    pushupmx(o);
    return o;
}

// cover: 对区间 [lt,rt] 做覆盖计数增/减（op=true 增，false 减）
void cover(int o, int lt, int rt, bool op) {
    int l = L[o], r = R[o];
    if (l >= lt && r <= rt) {
        cc[o] += (op ? 1 : -1);
        pushupc(o);
        return;
    }
    if (lt <= mid) cover(ls, lt, rt, op);
    if (rt > mid) cover(rs, lt, rt, op);
    pushupc(o);
}

// ins: 单点更新（链上序号 pos），op=true 插入 x，op=false 删除 x
void ins(int o, int pos, int x, bool op) {
    int l = L[o], r = R[o];
    if (l == r) {
        int u = dfns[l];
        op ? ms[u].ins(x) : ms[u].del(x); // 更新该节点的 mset
        mx[o][0] = ms[u].top() + s[u];
        mx[o][1] = ms[u].top() - s[u];
        return;
    }
    if (pos <= mid)
        ins(ls, pos, x, op);
    else
        ins(rs, pos, x, op);
    pushupmx(o);
}

// qmx0: 查询区间 [1, rt] 的 mx[][0] 最大值（用于 K-(S-s) 类型查询）
int qmx0(int o, int rt) {
    int l = L[o], r = R[o];
    if (r <= rt) return mx[o][0];
    int res = qmx0(ls, rt);
    if (rt > mid) res = max(res, qmx0(rs, rt));
    return res;
}

// qmx1: 查询区间 [pos, end] 的 mx[][1] 最大值（用于 K-(s-S) 类型查询）
int qmx1(int o, int pos) {
    int l = L[o], r = R[o];
    if (l >= pos) return mx[o][1];
    int res = qmx1(rs, pos);
    if (pos <= mid) res = max(res, qmx1(ls, pos));
    return res;
}

// isc: 检查区间 [lt,rt] 是否全部被覆盖（cov 标志）
bool isc(int o, int lt, int rt) {
    if (cov[o]) return 1; // 整个节点区间被标记为覆盖
    int l = L[o], r = R[o];
    if (l >= lt && r <= rt) return 0; // 当前区间未被 cov 标记且完全包含在查询区间内 -> 未覆盖
    if (lt <= mid)
        if (!isc(ls, lt, rt)) return 0;
    if (rt > mid)
        if (!isc(rs, lt, rt)) return 0;
    return 1;
}
} // namespace sgt

#undef ls
#undef rs
#undef mid

// Qmx(u): 计算节点 u 的贡献值（结合线段树两类查询）
// 公式：max( qmx0(root[u], dfn[u]) - s[u], qmx1(root[u], dfn[u]) + s[u] )
// 若 u==0 返回 -inf
int Qmx(int u) {
    return (!u) ? -inf : max(sgt::qmx0(root[u], dfn[u]) - s[u], sgt::qmx1(root[u], dfn[u]) + s[u]);
}

int n, m, q; // n:节点数, m:初始插入点数, q:查询数

// init: 读入树并做重链剖分与为每条链建立线段树
void init() {
    in(n), in(m), in(q);
    for (int i = 1, u, v, w; i < n; i++) {
        in(u), in(v), in(w);
        e[u].emplace_back(mkp(v, w));
        e[v].emplace_back(mkp(u, w));
    }
    dfs1(1, 0, 0, 0); // 从根 1 开始 dfs1
    dfs2(1, 1);       // dfs2 建链
    for (int i = 1; i <= n; i++) {
        ss[dfn[i]] = s[i]; // ss 按 dfn 顺序存储 s 值（链内用于二分）
        int u = dfns[i];
        root[u] = root[top[u]];                                     // 继承链顶的 root（如果已存在）
        if (u == top[u]) root[u] = sgt::build(dfn[u], dfn[bot[u]]); // 每个链顶建一棵线段树，区间为该链的 dfn 范围
    }
}

/*
  ins(u, K, op):
  将一个“以 u 为中心、半径为 K 的点”插入或删除（op=true 插入，false 删除）。
  处理方法：从 u 向上沿重链跳跃，每次在当前链上：
    - 在线段树的单点位置 dfn[u] 插入/删除 K（用于后续的最大值查询）
    - 在链上找到满足 |s[x]-s[u]| <= K 的连续区间 [l,r]（通过 ss 数组二分）
      并对该区间做 cover 标记（计数增/减）
    - 然后 K 减去跨到上链父节点所需的距离，继续向上直到 K < 0 或到根
*/
void ins(int u, int K, bool op) {
    while (u && K >= 0) {
        int tp = top[u], bt = bot[u], rt = root[u], md = dfn[u];
        // 单点更新：把 K 插入/删除到链上对应的线段树叶子（代表节点 u）
        sgt::ins(rt, md, K, op);

        // 在链 [tp..bt] 的 ss（按 dfn 排序的 s 值）上找到满足 |s[x]-s[u]| <= K 的区间
        int l = lower_bound(ss + dfn[tp], ss + dfn[bt] + 1, ss[md] - K) - ss;
        int r = upper_bound(ss + dfn[tp], ss + dfn[bt] + 1, ss[md] + K) - ss - 1;
        if (l <= r) sgt::cover(rt, l, r, op); // 对该区间做覆盖计数增/减

        // 跨到父链：K 减去从链顶到父节点的距离差（s[u] - s[fa[tp]]）
        K -= (s[u] - s[fa[tp]]);
        u = fa[tp];
    }
}

// 以下为 getans 所用的辅助栈与变量（用于分段处理路径）
int stkX[N], stkY[N], smxX[N], smxY[N], topx = 0, topy = 0;

/*
  getans(u, v): 判断路径 u-v 是否满足被当前插入的“带半径点”覆盖/连通条件
  主要思路：
    - 将 u->v 路径分解为若干链段（通过重链剖分）
    - 对每个链段使用线段树的查询（Qmx）与覆盖检查（isc）来判断是否满足条件
    - 过程中维护一些中间最大值 mxcov，用于判断链段内部是否被覆盖
    - 若所有链段都通过检查则返回 true（Yes），否则 false（No）
  该函数较复杂，直接按代码逻辑逐步注释。
*/
bool getans(int u, int v) {
    topx = topy = 0; // 清空栈
    // 先把两端分别向上跳到同一条链
    while (top[u] != top[v]) {
        if (d[top[u]] > d[top[v]]) {
            // u 端更深：把当前节点 u 入栈 X，跳到其链顶的父节点
            stkX[++topx] = u;
            u = fa[top[u]];
            // smxX[topx] 存储跳过该链后父节点的 Qmx 值（非负化）
            smxX[topx] = max(0, Qmx(u));
        } else {
            // v 端更深：把当前节点 v 入栈 Y，跳到其链顶的父节点
            stkY[++topy] = v;
            v = fa[top[v]];
            smxY[topy] = max(0, Qmx(v));
        }
    }

    // 现在 u 和 v 在同一条链上，令 lca = u（较浅者）
    if (d[u] > d[v]) swap(u, v);
    int lca = u;
    int mxcov = 0, lt, len = (s[v] - s[fa[u]]); // len 为 lca->v 的距离
    u = fa[top[u]];                             // 从 lca 的上一个链开始向上遍历

    // 计算从 lca 上方各链的最大覆盖贡献 mxcov
    while (u) {
        mxcov = max(mxcov, Qmx(u) - (s[fa[lca]] - s[u]));
        u = fa[top[u]];
    }

    u = lca;
    // 如果 mxcov < len，需要检查链上从 u 到 v 的某个前缀是否被覆盖
    if (mxcov < len) {
        lt = upper_bound(ss + dfn[u], ss + dfn[v] + 1, s[fa[u]] + mxcov) - ss;
        if (!sgt::isc(root[u], lt, dfn[v])) return 0; // 若该区间未被覆盖则失败
    }

    // 更新栈顶的 smx 值，调整为相对于 lca 的偏移
    chkmx(smxX[topx], mxcov - (s[fa[top[stkX[topx]]]] - s[fa[lca]]));
    chkmx(smxY[topy], mxcov - (s[fa[top[stkY[topy]]]] - s[fa[lca]]));

    // 处理 X 栈（u 端之前入栈的链段），逐段检查
    while (topx) {
        u = top[stkX[topx]];     // 链顶
        v = stkX[topx];          // 链内节点
        len = (s[v] - s[fa[u]]); // 链段长度
        mxcov = smxX[topx];      // 当前链段允许的最大覆盖
        if (mxcov < len) {
            lt = upper_bound(ss + dfn[u], ss + dfn[v] + 1, s[fa[u]] + mxcov) - ss;
            if (!sgt::isc(root[u], lt, dfn[v])) return 0;
        }
        topx--;
        // 将当前链段的剩余覆盖能力传递到上一段
        chkmx(smxX[topx], mxcov - len);
    }

    // 处理 Y 栈（v 端之前入栈的链段），逻辑同上
    while (topy) {
        u = top[stkY[topy]];
        v = stkY[topy];
        len = (s[v] - s[fa[u]]);
        mxcov = smxY[topy];
        if (mxcov < len) {
            lt = upper_bound(ss + dfn[u], ss + dfn[v] + 1, s[fa[u]] + mxcov) - ss;
            if (!sgt::isc(root[u], lt, dfn[v])) return 0;
        }
        topy--;
        chkmx(smxY[topy], mxcov - len);
    }

    return 1; // 所有链段检查通过
}

// P/K 数组用于记录插入的点（方便删除时引用）
int P[N << 1], K[N << 1];

// work: 处理初始 m 个插入，然后处理 q 个操作（插入/删除/查询）
void work() {
    for (int i = 1; i <= m; i++) in(P[i]), in(K[i]), ins(P[i], K[i], 1); // 初始插入
    int op, x, y;
    while (q--) {
        in(op), in(x);
        if (op != 2) in(y); // op==2 是删除操作只需要索引 x
        if (op == 1) {
            // 插入新点
            ++m;
            P[m] = x;
            K[m] = y;
            ins(x, y, 1);
        }
        if (op == 2) {
            // 删除第 x 个插入的点（使用之前记录的 P[x], K[x]）
            ins(P[x], K[x], 0);
        }
        if (op == 3) {
            // 查询 x,y 是否满足条件
            getans(x, y) ? printf("Yes\n") : printf("No\n");
        }
    }
}

int main() {
    // 文件重定向（竞赛/测试用），实际使用时可注释或改为标准输入输出
    freopen("secure.in", "r", stdin);
    freopen("secure.out", "w", stdout);
    init();
    work();
    return 0;
}
