#include <cstdio>
#include <cstring>
#include <queue>
constexpr int N = 100005;
std::vector<int> g[N];
struct PQ {
    int top() { while (allel.top() == delel.top()) allel.pop(), delel.pop(); return allel.top(); }
    int pop() { int res = top(); allel.pop(); return res; }
    void del(int x) { delel.push(x); }
    void ins(int x) { allel.push(x); }
private: std::priority_queue<int> allel, delel;
};
bool vis[N];
namespace PntDivide {
    int centroid;
    int sz[N];
    void dfs(int u, int fa) {
        for (int v : g[u]) if (!vis[v] && v != fa) {

        }
    }
    void getCentroid(int u, int fa) {
        
    }
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    return 0;
}