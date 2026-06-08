#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

const int N = 1e5 + 5;

// 原树邻接表 & 点分树邻接表
vector<int> adj[N], cent_tree[N];

int n, m;
int rt;                    // 当前找重心时的候选根
int subtree_size[N];       // 子树大小
int max_sub_sz[N];         // 找重心时用的最大子树大小
int tot;                   // 当前处理的子树节点总数
bool visited[N];           // 点分治中已处理的节点
int answer[N];             // 每个查询的最终答案
int fenwick[N];            // 树状数组（维护min_id位置的颜色计数）
int last_color_pos[N];     // 每个颜色当前记录的最大编号位置（用于去重）
int color[N];              // 节点颜色
int rollback_st[N], rollback_cnt;  // 用于回滚树状数组的栈

struct Query {
    int id, L, R;          // 查询编号, 区间[l,r]
};
vector<Query> queries_per_node[N];   // 每个节点x对应的查询列表
vector<Query> active_nodes, active_queries;  // p和w

inline int lowbit(int x) { return x & (-x); }

void fenwick_update(int x, int val) {
    while (x <= n) {
        fenwick[x] += val;
        x += lowbit(x);
    }
}

int fenwick_query(int x) {
    int sum = 0;
    while (x) {
        sum += fenwick[x];
        x -= lowbit(x);
    }
    return sum;
}

bool cmp_by_R(const Query &a, const Query &b) {
    return a.R < b.R;
}

// ====================== 点分治建树 ======================

// 计算子树大小并寻找重心
void compute_size_and_centroid(int u, int parent) {
    subtree_size[u] = 1;
    max_sub_sz[u] = 1;
    
    for (int v : adj[u]) {
        if (visited[v] || v == parent) continue;
        compute_size_and_centroid(v, u);
        subtree_size[u] += subtree_size[v];
        max_sub_sz[u] = max(max_sub_sz[u], subtree_size[v]);
    }
    max_sub_sz[u] = max(max_sub_sz[u], tot - subtree_size[u]);
    
    if (max_sub_sz[rt] > max_sub_sz[u]) rt = u;
}

// 构建点分树
void build_centroid_tree(int centroid, int subtree_nodes) {
    visited[centroid] = true;
    
    for (int v : adj[centroid]) {
        if (visited[v]) continue;
        
        if (subtree_size[centroid] > subtree_size[v])
            tot = subtree_size[v];
        else
            tot = subtree_nodes - subtree_size[centroid];
        
        rt = 0;
        compute_size_and_centroid(v, 0);
        
        cent_tree[centroid].push_back(rt);   // 点分树父子关系
        build_centroid_tree(rt, tot);
    }
}

// ====================== 查询处理核心 ======================

// DFS原树，记录路径上的编号范围 [min_id, max_id]
void dfs_collect(int u, int parent, int min_id, int max_id) {
    active_nodes.push_back({color[u], min_id, max_id});
    
    // 收集当前节点u能回答的查询（路径范围完全包含查询区间）
    for (auto &qry : queries_per_node[u]) {
        if (!answer[qry.id] && min_id >= qry.L && max_id <= qry.R) {
            active_queries.push_back(qry);
        }
    }
    
    for (int v : adj[u]) {
        if (v == parent || visited[v]) continue;
        dfs_collect(v, u, min(min_id, v), max(max_id, v));
    }
}

// 处理以当前点分治中心u为最高点的连通块相关查询
void process_centroid(int u) {
    active_nodes.clear();
    active_queries.clear();
    
    dfs_collect(u, 0, u, u);   // 从u开始，初始路径min=max=u
    
    sort(active_nodes.begin(), active_nodes.end(), cmp_by_R);
    sort(active_queries.begin(), active_queries.end(), cmp_by_R);
    
    int ptr = 0;
    int nodes_cnt = active_nodes.size();
    int q_cnt = active_queries.size();
    
    for (int i = 0; i < q_cnt; i++) {
        // 将所有 max_id <= 当前查询R 的节点加入数据结构
        while (ptr < nodes_cnt && active_nodes[ptr].R <= active_queries[i].R) {
            int col = active_nodes[ptr].id;      // id字段存颜色
            int pos = active_nodes[ptr].L;       // L字段存路径最小编号
            
            if (!last_color_pos[col]) {
                fenwick_update(pos, 1);
                last_color_pos[col] = pos;
                rollback_st[++rollback_cnt] = col;
            } else if (pos > last_color_pos[col]) {
                // 保留编号更大的位置（贡献范围更大）
                fenwick_update(last_color_pos[col], -1);
                fenwick_update(pos, 1);
                last_color_pos[col] = pos;
            }
            ptr++;
        }
        
        // 查询颜色种类数：路径min_id在 [L, n] 的不同颜色
        answer[active_queries[i].id] = fenwick_query(n) - fenwick_query(active_queries[i].L - 1);
    }
    
    // 回滚树状数组
    while (rollback_cnt) {
        int col = rollback_st[rollback_cnt];
        fenwick_update(last_color_pos[col], -1);
        last_color_pos[col] = 0;
        rollback_cnt--;
    }
}

// 在点分树上DFS处理每个中心
void dfs_cent_tree(int u) {
    process_centroid(u);
    visited[u] = true;
    
    for (int v : cent_tree[u]) {
        dfs_cent_tree(v);
    }
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &color[i]);
    
    for (int i = 1; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    
    // 寻找整棵树的重心并构建点分树
    max_sub_sz[0] = 2e9;
    tot = n;
    compute_size_and_centroid(1, 0);
    int centroid_root = rt;
    
    for (int i = 1; i <= n; i++) visited[i] = false;
    build_centroid_tree(centroid_root, n);
    
    // 读入所有查询，按查询节点x分组存储
    for (int i = 1; i <= m; i++) {
        int L, R, x;
        scanf("%d%d%d", &L, &R, &x);
        queries_per_node[x].push_back({i, L, R});
    }
    
    // 重置visited，在点分树上处理查询
    for (int i = 1; i <= n; i++) visited[i] = false;
    dfs_cent_tree(centroid_root);
    
    for (int i = 1; i <= m; i++) {
        printf("%d\n", answer[i]);
    }
    return 0;
}