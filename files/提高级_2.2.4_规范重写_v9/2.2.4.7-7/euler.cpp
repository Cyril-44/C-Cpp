#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 5;
const int MAXM = 2e5 + 5;

struct edge{
    int v, id;
};

vector<edge> G[MAXN];
bool used[MAXM];
int n, m, deg[MAXN];
vector<int> pos, ans;

void dfs(int u){
    for (int &i = pos[u]; i < int(G[u].size()); ){
        edge e = G[u][i++];
        if (used[e.id]) continue;
        used[e.id] = true;
        dfs(e.v);
    }
    // 回溯时加入答案，得到的是欧拉回路的逆序
    ans.push_back(u);
}

vector<int> findEuler(){
    int start = 1;
    // 欧拉回路要求所有顶点度数为偶数
    for (int u = 1; u <= n; u++){
        if (deg[u] % 2 != 0) return {};
        if (deg[u] > 0) start = u;
    }
    if (m == 0) return {1};

    fill(used + 1, used + m + 1, false);
    pos.assign(n + 1, 0);
    ans.clear();
    dfs(start);

    // 少于 m + 1 个点说明非零度部分不连通
    if (int(ans.size()) != m + 1) return {};
    reverse(ans.begin(), ans.end());
    return ans;
}

int main() {
    
}