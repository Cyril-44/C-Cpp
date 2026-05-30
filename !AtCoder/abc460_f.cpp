#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
template <class T> void in(T &x) {
    char c = getchar();
    T f = 1;
    x = 0;
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 100010;
int lgv[N];
void init() {
    lgv[1] = 0;
    for (int i = 2; i < N; i++) lgv[i] = lgv[i >> 1] + 1;
}
struct Heap {
    priority_queue<int> a, b;
    void remove() {
        while (!b.empty()) {
            if (a.top() == b.top())
                a.pop(), b.pop();
            else
                break;
        }
    }
    int size() { return a.size() - b.size(); }
    bool empty() { return a.size() == b.size(); }
    void insert(int x) {
        if (x != -1) a.push(x);
    }
    void erase(int x) {
        if (x != -1) b.push(x);
    }
    int top() {
        remove();
        return empty() ? -1 : a.top();
    }
    void pop() {
        remove();
        a.pop();
    }
    int stop() {
        if (size() <= 1) return -1;
        int tmp = top(), tmp2;
        pop();
        tmp2 = top();
        a.push(tmp);
        return tmp2;
    }
    int answer() {
        if (size() <= 1) return -1;
        return top() + stop();
    }
} son[N], fdis[N], ans;
int n, m, u, v;
char op[2];
vector<int> g[N], h[N];
int dfn[N], dep[N], ck, st[17][N], ff[N], fa[N];
int higher(int u, int v) {
    return dep[u] < dep[v] ? u : v;
}
void inittree(int u, int fa) {
    dfn[u] = ++ck;
    st[0][ck] = u;
    ff[u] = fa;
    for (int v : g[u]) {
        if (v == fa) continue;
        dep[v] = dep[u] + 1;
        inittree(v, u);
    }
}
void initst() {
    for (int i = 1; i <= 16; i++)
        for (int j = 1; j + (1 << i) - 1 <= n; j++) st[i][j] = higher(st[i - 1][j], st[i - 1][j + (1 << i - 1)]);
}
int lca(int u, int v) {
    if (u == v) return u;
    u = dfn[u];
    v = dfn[v];
    if (u > v) swap(u, v);
    u++;
    int l = lgv[v - u + 1];
    return ff[higher(st[l][u], st[l][v - (1 << l) + 1])];
}
int dis(int u, int v) {
    return dep[u] + dep[v] - (dep[lca(u, v)] << 1);
}
bool vis[N];
int sz[N], mxsz[N], rt;
vector<int> l;
void getcenter(int u, int fa) {
    sz[u] = 1;
    l.push_back(u);
    mxsz[u] = 0;
    for (int v : g[u]) {
        if (v == fa || vis[v]) continue;
        getcenter(v, u);
        sz[u] += sz[v];
        mxsz[u] = max(mxsz[u], sz[v]);
    }
}
int divide(int u) {
    l.clear();
    getcenter(u, 0);
    int cur = l[0];
    for (int x : l) {
        mxsz[x] = max(mxsz[x], sz[u] - sz[x]);
        if (mxsz[cur] > mxsz[x]) cur = x;
    }
    vis[cur] = 1;
    // printf("%d's center is %d\n",u,cur);
    for (int v : g[cur]) {
        if (vis[v]) continue;
        h[cur].push_back(divide(v));
        fa[h[cur].back()] = cur;
    }
    return cur;
}
bool lgt[N];
int tot;
void reverse(int u) {
    if (!lgt[u]) {
        tot--;
        ans.erase(son[u].answer());
        son[u].erase(0);
        ans.insert(son[u].answer());
        for (int i = u; fa[i]; i = fa[i]) {
            int rem = dis(u, fa[i]);
            // printf("- %d %d %d\n",u,fa[i],dis(u,fa[i]));
            ans.erase(son[fa[i]].answer());
            son[fa[i]].erase(fdis[i].top());
            fdis[i].erase(rem);
            son[fa[i]].insert(fdis[i].top());
            ans.insert(son[fa[i]].answer());
        }
    } else {
        tot++;
        ans.erase(son[u].answer());
        son[u].insert(0);
        ans.insert(son[u].answer());
        for (int i = u; fa[i]; i = fa[i]) {
            int rem = dis(u, fa[i]);
            // printf("+ %d %d %d\n",u,fa[i],dis(u,fa[i]));
            ans.erase(son[fa[i]].answer());
            son[fa[i]].erase(fdis[i].top());
            fdis[i].insert(rem);
            son[fa[i]].insert(fdis[i].top());
            ans.insert(son[fa[i]].answer());
        }
    }
    lgt[u] ^= 1;
}
int main() {
    init();
    in(n);
    for (int i = 1; i <= n; i++) lgt[i] = 1;
    for (int i = 1; i < n; i++) {
        in(u);
        in(v);
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    inittree(1, 0);
    initst();
    rt = divide(1);
    // print(rt);
    for (int i = 1; i <= n; i++) reverse(i);
    in(m);
    for (int i = 1; i <= m; i++) {
        in(u);
        reverse(u);
        printf("%d\n", ans.top());
    }
    return 0;
}