// 最小值函数：返回 x 和 y 中的较小者
int min(int x, int y) {
    return x < y ? x : y; // 三元运算：若 x<y 返回 x，否则返回 y
}

// 常量与全局变量
const int N = 3e5 + 10;   // 数组容量上界，支持约 3e5 个节点
const int INF = 1e9 + 10; // 代表“无穷大”的常量，用于初始化最短距离

int n, m, top, cnt; // 通用变量：n/m（节点数/关键点数），top（虚树栈顶），cnt（通用计数器）
int dep[N];         // 原树中每个节点的深度（根到该节点的深度）
int f[N][20];       // 倍增表：f[u][i] 表示 u 的 2^i 级祖先（用于 LCA / 跳祖）
int dfn[N];         // 原树 DFS 序号（时间戳），用于按序构造虚树
int size[N];        // 原树中以该节点为根的子树大小（节点数）
int lg2[N];         // 预处理的对数表：lg2[k] = floor(log2(k))
int sta[N];         // 构造虚树时使用的栈（存放节点序列）
int pnt[N];         // 临时点数组，通常存放要插入虚树的关键点集合
int vis[N];         // 标记数组：表示该节点是否为源点（关键点）或已被激活
int g[N];           // 每个节点当前最近的源点编号（来源点 id）
int dp[N];          // 每个节点到其最近源点的距离（在虚树上的距离度量）
int ans[N];         // 最终答案：ans[i] 表示源点 i 在原树中覆盖的节点数（或贡献）
int tmp[N];         // 临时数组，供中间计算或排序使用

// 图的邻接表结构（前向星），用于表示原树和虚树
struct graph {
    int tot;        // 边数组已使用的条目数（每条无向边占两条记录）
    int fir[N];     // fir[u] 指向以 u 为起点的第一条边的索引（邻接表头）
    int to[2 * N];  // to[i] 表示第 i 条边的目标节点
    int nxt[2 * N]; // nxt[i] 表示第 i 条边在链表中的下一个边索引
    // 添加无向边 x <-> y（在邻接表中插入两条记录）
    inline void add(int x, int y) {
        to[++tot] = y;     // 插入 x -> y
        nxt[tot] = fir[x]; // 将新边接到 x 的链表头
        fir[x] = tot;      // 更新 x 的链表头为新边索引
        to[++tot] = x;     // 插入 y -> x（无向边的另一半）
        nxt[tot] = fir[y]; // 将新边接到 y 的链表头
        fir[y] = tot;      // 更新 y 的链表头为新边索引
    }
} e1, e2; // e1 通常表示原树，e2 表示构造的虚树

// cal 函数：在虚树边 (x,y) 上把原树节点的贡献分配给对应的源点
inline void cal(int o, int u) {
    int p = u, dvd = u; // p 用于找到 y 在原树上对应 x 的直接子节点；q 用于找到路径分割点

    // 将 p 向上跳，直到 p 停在 y->...->x 路径上且是 x 的直接子节点（即 p 的父节点是 x 或深度刚好比 x 大）
    for (int i = lg2[dep[p]]; i >= 0; i--)
        if (dep[f[p][i]] > dep[o]) p = f[p][i];
    
    // 在 y->x 路径上用倍增寻找分割点 q：
    // 对于候选祖先 t = f[q][i]，比较由 y 的源点覆盖到 t 的代价（llen）
    // 与由 x 的源点覆盖到 t 的代价（rlen），若 y 更优则把 q 上跳到 t
    for (int i = lg2[dep[dvd]]; i >= 0; i--) {
        int llen = dep[u] - dep[f[dvd][i]] + dp[u]; // y 的源点覆盖到 f[q][i] 的总代价
        int rlen = dep[f[dvd][i]] - dep[o] + dp[o]; // x 的源点覆盖到 f[q][i] 的总代价
        // 保证不跳到 x 之上；若 y 更优（距离更短或编号更小），则 q 上跳
        if (dep[f[dvd][i]] > dep[o] && (llen < rlen || (llen == rlen && g[u] < g[o]))) dvd = f[dvd][i];
    }
    // 分割后：
    // size[q] - size[y] 属于 g[y]（y 侧源点覆盖的原树节点数）
    // size[p] - size[q] 属于 g[x]（x 侧源点覆盖的原树节点数）
    ans[g[u]] += size[dvd] - size[u];
    // 此时 p 是 y 在原树上对应的 x 的儿子（或路径上最靠近 x 的那个节点）
    ans[g[o]] -= size[p]; // 从 g[x] 的答案中减去 size[p]：因为这部分子树更靠近 y 的源点，不应计入 g[x]
    ans[g[o]] += size[p] - size[dvd];
}

// dfs1：虚树上的自底向上 DP，合并子节点的最近源点信息
inline void dfs1(int p, int fa) {
    dp[p] = INF; // 初始化 p 的距离为无穷大（表示暂未找到来源）
    // 遍历虚树 e2 中 p 的所有邻居（子节点）
    for (int i = e2.fir[p]; i; i = e2.nxt[i]) {
        int q = e2.to[i];
        if (q == fa) continue;     // 跳过父节点，保证为树的遍历
        dfs1(q, p);                // 递归处理子节点 q
        int dis = dep[q] - dep[p]; // 虚树边权：原树中 q 与 p 的深度差（可能 >1）
        // 通过子节点 q 的来源更新 p 的最短来源信息
        if (dp[q] + dis < dp[p])
            dp[p] = dp[q] + dis, g[p] = g[q]; // 更短则继承 q 的来源
        else if (dp[q] + dis == dp[p])
            g[p] = min(g[p], g[q]); // 相等则取编号更小的来源
    }
    // 若 p 本身是源点（关键点），则它到自己的距离为 0，来源为自己
    if (vis[p]) dp[p] = 0, g[p] = p;
}

// dfs2：虚树上的自顶向下 DP，把父节点信息传给子节点，并在遍历时分配贡献
inline void dfs2(int p, int fa) {
    // 遍历 p 的子节点
    for (int i = e2.fir[p]; i; i = e2.nxt[i]) {
        int q = e2.to[i];
        if (q == fa) continue;     // 跳过父节点
        int dis = dep[q] - dep[p]; // 虚树边权（深度差）
        // 父节点 p 的来源可能通过边传递给子节点 q，更新 q 的 dp/g
        if (dp[p] + dis < dp[q])
            dp[q] = dp[p] + dis, g[q] = g[p]; // 更短则继承 p 的来源
        else if (dp[p] + dis == dp[q])
            g[q] = min(g[q], g[p]); // 相等则取编号更小的来源
        cal(p, q);                  // 在虚树边 (p,q) 上分配原树节点贡献（把区间贡献计入 ans）
        dfs2(q, p);                 // 递归处理子节点 q
    }
    ans[g[p]] += size[p]; // 把以 p 为根的原树子树全部计入其最终来源 g[p]
    // 清理标记，便于下一次构造虚树或复用数组
    vis[p] = 0;    // 取消源点标记（或表示已处理）
    e2.fir[p] = 0; // 清空虚树邻接表头，释放该节点在虚树中的边信息
}
