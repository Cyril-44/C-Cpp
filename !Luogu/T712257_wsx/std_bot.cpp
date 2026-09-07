#include <bits/stdc++.h>
using namespace std;

static const uint64_t RANDOM_CONST = chrono::steady_clock::now().time_since_epoch().count();

struct FastHash {
    size_t operator()(uint64_t x) const {
        x ^= RANDOM_CONST;
        x ^= x >> 23;
        x *= 0x2127599bf4325c37ULL;
        x ^= x >> 47;
        return x;
    }
};

const int MAXN = 200000 + 5;

vector<int> g[MAXN];
int n;
int s[MAXN];
int sz[MAXN], heavy[MAXN];
int ans[MAXN];

// DSU 结构
unordered_map<int,int,FastHash> seg; // 记录每个值所在连续段的长度（仅端点有效）
unordered_set<int,FastHash> exist;   // 是否出现过
int best = 1;

// 插入一个值 v
void insert_val(int v) {
    if (exist.count(v)) return;

    int left = exist.count(v - 1) ? seg[v - 1] : 0;
    int right = exist.count(v + 1) ? seg[v + 1] : 0;

    int total = left + 1 + right;

    // 新段的左右端点
    int L = v - left;
    int R = v + right;

    seg[L] = seg[R] = total;
    exist.insert(v);

    best = max(best, total);
}

// 预处理子树大小 + 重儿子
void dfs_sz(int u, int p) {
    sz[u] = 1;
    heavy[u] = -1;
    for (int v : g[u]) if (v != p) {
        dfs_sz(v, u);
        sz[u] += sz[v];
        if (heavy[u] == -1 || sz[v] > sz[heavy[u]])
            heavy[u] = v;
    }
}

// 把子树所有点的值插入（不清空）
void add_subtree(int u, int p) {
    insert_val(s[u]);
    for (int v : g[u]) if (v != p)
        add_subtree(v, u);
}

// 清空子树（用于小并大）
void clear_subtree(int u, int p) {
    exist.erase(s[u]);
    seg.erase(s[u]);
    for (int v : g[u]) if (v != p)
        clear_subtree(v, u);
}

// DSU on Tree
void dfs(int u, int p, bool keep) {
    for (int v : g[u]) if (v != p && v != heavy[u])
        dfs(v, u, false);

    if (heavy[u] != -1)
        dfs(heavy[u], u, true);

    for (int v : g[u]) if (v != p && v != heavy[u])
        add_subtree(v, u);

    insert_val(s[u]);
    ans[u] = best;

    if (!keep) {
        clear_subtree(u, p);
        best = 1;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) cin >> s[i];

    dfs_sz(1, 0);
    dfs(1, 0, true);

    for (int i = 1; i <= n; i++)
        cout << ans[i] << (i == n ? '\n' : ' ');
}
