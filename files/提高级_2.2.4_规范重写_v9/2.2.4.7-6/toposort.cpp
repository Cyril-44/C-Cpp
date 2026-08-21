#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> G[105];
vector<int> topoSort(){
    queue<int> q;
    vector<int> ans, deg(n+1);
    ans.reserve(n);
    for (int u = 1; u <= n; u++)
        for (int v : G[u])
            deg[v]++;
    for (int u = 1; u <= n; u++){
        if (deg[u] == 0) q.push(u);
    }

    while (!q.empty()){
        int u = q.front();
        q.pop();
        ans.push_back(u);
        // 删除 u 的所有出边，新产生的零入度节点随后入队
        for (int v : G[u]){
            deg[v]--;
            if (deg[v] == 0) q.push(v);
        }
    }
    // 有节点未被取出，等价于图中仍存在有向环
    if (int(ans.size()) != n) ans.clear();
    return ans;
}
int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        while (true) {
            int v; cin >> v;
            if (!v) break;
            G[i].push_back(v);
        }
    }
    auto res = topoSort(G, n);
    for (int p : res)
        cout << p << ' ';
}