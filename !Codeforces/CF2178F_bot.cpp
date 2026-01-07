#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n; cin >> n;
        vector<vector<int>> g(n+1);
        for (int i = 0; i < n-1; ++i) {
            int u,v; cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        // BFS to get parent order (root = 1)
        vector<int> parent(n+1, -2);
        vector<int> order;
        order.reserve(n);
        queue<int> q;
        parent[1] = -1;
        q.push(1);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : g[u]) if (parent[v] == -2) {
                parent[v] = u;
                q.push(v);
            }
        }

        // compute subtree sizes
        vector<int> sz(n+1, 1);
        for (int i = (int)order.size()-1; i >= 0; --i) {
            int u = order[i];
            for (int v : g[u]) if (v != parent[u]) {
                sz[u] += sz[v];
            }
        }

        // mark white vertices
        vector<int> isWhite(n+1, 0);
        int whiteCount = 0;
        for (int i = 1; i <= n; ++i) {
            if (sz[i] % 2 == 0) { isWhite[i] = 1; ++whiteCount; }
        }
        if (whiteCount == 0) {
            cout << 1 << '\n';
            continue;
        }

        // Contract black components: white vertices get unique comp ids, black connected components get their own ids
        vector<int> comp(n+1, -1);
        int compId = 0;
        // assign white vertices their own comp id
        for (int i = 1; i <= n; ++i) if (isWhite[i]) comp[i] = compId++;
        // flood fill black components
        for (int i = 1; i <= n; ++i) {
            if (!isWhite[i] && comp[i] == -1) {
                int id = compId++;
                // iterative stack
                stack<int> st;
                st.push(i);
                comp[i] = id;
                while (!st.empty()) {
                    int u = st.top(); st.pop();
                    for (int v : g[u]) {
                        if (!isWhite[v] && comp[v] == -1) {
                            comp[v] = id;
                            st.push(v);
                        }
                    }
                }
            }
        }

        // build contracted graph adjacency (use vector< vector<int> > then dedupe)
        vector<vector<int>> cg(compId);
        for (int u = 1; u <= n; ++u) {
            for (int v : g[u]) {
                if (comp[u] != comp[v]) {
                    cg[comp[u]].push_back(comp[v]);
                }
            }
        }
        // deduplicate adjacency lists
        for (int i = 0; i < compId; ++i) {
            sort(cg[i].begin(), cg[i].end());
            cg[i].erase(unique(cg[i].begin(), cg[i].end()), cg[i].end());
        }

        int rootComp = comp[1];

        // BFS on contracted graph to get parent pointers (rooted at rootComp)
        vector<int> parC(compId, -1), dist(compId, -1);
        queue<int> qc;
        qc.push(rootComp);
        dist[rootComp] = 0;
        parC[rootComp] = -1;
        while (!qc.empty()) {
            int u = qc.front(); qc.pop();
            for (int v : cg[u]) if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                parC[v] = u;
                qc.push(v);
            }
        }

        // collect distinct white component ids
        vector<int> whiteComps;
        whiteComps.reserve(whiteCount);
        for (int i = 1; i <= n; ++i) if (isWhite[i]) whiteComps.push_back(comp[i]);
        sort(whiteComps.begin(), whiteComps.end());
        whiteComps.erase(unique(whiteComps.begin(), whiteComps.end()), whiteComps.end());

        // mark nodes on paths from each whiteComp to rootComp
        vector<int> mark(compId, 0);
        for (int wc : whiteComps) {
            int cur = wc;
            while (cur != -1 && !mark[cur]) {
                mark[cur] = 1;
                cur = parC[cur];
            }
        }

        // check marked subgraph is a simple path (each marked node has <=2 marked neighbors)
        bool ok = true;
        int markedNodes = 0;
        for (int u = 0; u < compId; ++u) if (mark[u]) {
            ++markedNodes;
            int deg = 0;
            for (int v : cg[u]) if (mark[v]) ++deg;
            if (deg > 2) { ok = false; break; }
        }
        if (!ok) {
            cout << 0 << '\n';
            continue;
        }

        // find endpoints of the marked path (deg <=1 within marked subgraph)
        vector<int> endComps;
        for (int u = 0; u < compId; ++u) if (mark[u]) {
            int deg = 0;
            for (int v : cg[u]) if (mark[v]) ++deg;
            if (deg <= 1) endComps.push_back(u);
        }

        // count original vertices in each comp
        vector<int> cntComp(compId, 0);
        for (int i = 1; i <= n; ++i) cntComp[comp[i]]++;

        ll ans = 0;
        if ((int)endComps.size() == 0) {
            // shouldn't happen if markedNodes>0, but handle defensively
            cout << 0 << '\n';
            continue;
        } else if ((int)endComps.size() == 1) {
            // single node path: all whites in one comp (or single marked node)
            ans = cntComp[endComps[0]];
        } else {
            // two ends: sum sizes
            ans = ( (ll)cntComp[endComps[0]] + (ll)cntComp[endComps[1]] ) % MOD;
        }
        cout << (ans % MOD + MOD) % MOD << '\n';
    }
    return 0;
}
