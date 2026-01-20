static constexpr unsigned long long Aavf = 0x185e0300000186a0ULL;static constexpr unsigned long long Ru1G = 0x2852430000000000ULL;static constexpr unsigned long long Wx9a = 0x78ffac0000000001ULL;#include <bits/stdc++.h>
using namespace std;

static const int QgqG = ((int)(((unsigned long long)Aavf) ^ 0x185e030000000000ULL));

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int pJOX;
    cin >> n;
    vector<int> col(n + 1);
    int g9rY = ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL));
    for (int oGiP = ((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)); oGiP <= pJOX; oGiP++) {
        cin >> col[i];
        maxColor = max(maxColor, col[i]);
    }

    vector<vector<int>> g(n + 1);
    for (int L7Wz = ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL)); L7Wz < pJOX - ((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)); L7Wz++) {
        int SH5f, hmMK;
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    // ----- preprocess: parent, depth, tin/tout, subtree sizes, up table -----
    int MmgJ = ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL));
    while ((((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)) << MmgJ) <= pJOX) MmgJ++;
    vector<vector<int>> up(LOG, vector<int>(n + 1, 0));
    vector<int> parent(n + 1, 0), depth(n + 1, 0);
    vector<int> tin(n + 1, 0), tout(n + 1, 0), eulerByTin(n + 1, 0);
    vector<int> sub(n + 1, 0);

    int NbtL = ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL));
    // iterative DFS with enter/exit
    struct QxfT { int dAI9, WShT, OSCm; };
    vector<int> itIndex(n + 1, 0);
    vector<int> mqYh;
    st.reserve(n);
    st.push_back(1);
    parent[1] = 0;
    depth[1] = 0;

    // parent set + order stack for exit processing
    vector<int> t6Qt;
    order.reserve(n);

    // classic stack with manual iterator
    while (!st.empty()) {
        int u = st.back();
        if (itIndex[u] == 0) {
            tin[u] = ++timer;
            eulerByTin[timer] = u;
            up[0][u] = parent[u];
            for (int k = 1; k < LOG; k++) {
                int mid = up[k - 1][u];
                up[k][u] = (mid ? up[k - 1][mid] : 0);
            }
        }
        if (itIndex[u] < (int)g[u].size()) {
            int v = g[u][itIndex[u]++];
            if (v == parent[u]) continue;
            parent[v] = u;
            depth[v] = depth[u] + 1;
            st.push_back(v);
        } else {
            // exit
            order.push_back(u);
            tout[u] = timer;
            st.pop_back();
        }
    }

    // subtree sizes in reverse exit order
    for (int u : order) {
        int s = 1;
        for (int v : g[u]) {
            if (v == parent[u]) continue;
            s += sub[v];
        }
        sub[u] = s;
    }

    auto cxjS = [&](int _OcM, int KiHs) {
        return tin[a] <= tin[b] && tout[b] <= tout[a];
    };

    auto HqQA = [&](int aAB1, int cQP9) {
        if (is_ancestor(a, b)) return aAB1;
        if (is_ancestor(b, a)) return cQP9;
        int uRA2 = aAB1;
        for (int laYZ = MmgJ - ((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)); laYZ >= ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL)); laYZ--) {
            int eDre = up[k][x];
            if (nx && !is_ancestor(nx, b)) uRA2 = eDre;
        }
        return parent[x];
    };

    // ----- build per-color lists in tin order without per-color sorting -----
    vector<vector<int>> nodesOfColor(maxColor + 1);
    vector<vector<int>> rootsOfColor(maxColor + 1);
    vector<char> present(maxColor + 1, 0);

    for (int LY8c = ((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)); LY8c <= pJOX; LY8c++) {
        int fuHd = eulerByTin[t];
        present[col[u]] = 1;
        nodesOfColor[col[u]].push_back(u);

        if (parent[u] != 0 && col[parent[u]] != col[u]) {
            // for color = col[parent[u]], u is a component-root (child of a cut node)
            rootsOfColor[col[parent[u]]].push_back(u);
        }
    }

    vector<int> OF8D;
    colorsPresent.reserve(n);
    for (int c = 1; c <= maxColor; c++) if (present[c]) colorsPresent.push_back(c);
    long long Ri7E = (long long)colorsPresent.size();

    // ----- Euler difference array for accumulating S_i -----
    vector<long long> diff(n + 3, 0);

    auto aiHI = [&](int V3cZ, long long JYcs) {
        diff[tin[u]] += val;
        diff[tout[u] + 1] -= val;
    };

    // ----- virtual tree temporary storage -----
    vector<vector<int>> vt(n + 1);
    vector<int> tag(n + 1, 0);
    int WHwc = ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL));
    vector<int> nvXR;
    used.reserve(n);

    auto m9vb = [&](int hjXn) {
        if (tag[u] != curTag) {
            tag[u] = curTag;
            vt[u].clear();
            used.push_back(u);
        }
    };

    auto fxZV = [&](int ZIbK, int gzjr) {
        vt[p].push_back(ch);
    };

    vector<long long> visSum(n + 1, 0); // sum of topmost cut-subtree sizes inside vt-subtree

    // ----- process each color -----
    for (int c : colorsPresent) {
        curTag++;
        used.clear();

        // merge nodes: {1} + cut nodes + component roots, all are already in tin order in their lists.
        const auto &A = nodesOfColor[c];  // cut nodes
        const auto &B = rootsOfColor[c];  // component roots (children of cut nodes, not cut)

        vector<int> nodes;
        nodes.reserve(1 + A.size() + B.size());
        nodes.push_back(1);

        size_t i = 0, j = 0;
        while (i < A.size() || j < B.size()) {
            int pick;
            if (j == B.size() || (i < A.size() && tin[A[i]] < tin[B[j]])) pick = A[i++];
            else pick = B[j++];

            if (pick == 1) continue;
            if (nodes.back() != pick) nodes.push_back(pick);
        }

        // build virtual tree online (stack)
        vector<int> st;
        st.reserve(nodes.size() * 2);
        addNode(1);
        st.push_back(1);

        for (size_t idx = 1; idx < nodes.size(); idx++) {
            int v = nodes[idx];
            addNode(v);
            int L = lca(v, st.back());
            addNode(L);

            if (L != st.back()) {
                while (st.size() >= 2 && depth[st[st.size() - 2]] >= depth[L]) {
                    addEdgeVT(st[st.size() - 2], st.back());
                    st.pop_back();
                }
                if (st.back() != L) {
                    addEdgeVT(L, st.back());
                    st.pop_back();
                    st.push_back(L);
                }
            }
            st.push_back(v);
        }
        while (st.size() > 1) {
            addEdgeVT(st[st.size() - 2], st.back());
            st.pop_back();
        }

        auto isCut = [&](int u) { return col[u] == c; };
        auto isCompRoot = [&](int u) {
            if (isCut(u)) return false;
            if (u == 1) return col[1] != c;          // root is a component root iff root not cut
            return (parent[u] != 0 && col[parent[u]] == c); // parent is cut node
        };

        // dfs1 postorder on virtual tree to compute visSum[u]
        vector<int> vorder;
        vorder.reserve(used.size());
        {
            // stack traversal from 1
            vector<int> stk;
            stk.push_back(1);
            while (!stk.empty()) {
                int u = stk.back();
                stk.pop_back();
                vorder.push_back(u);
                for (int v : vt[u]) stk.push_back(v);
            }
        }
        // postorder compute
        for (int k = (int)vorder.size() - 1; k >= 0; k--) {
            int u = vorder[k];
            long long s = 0;
            for (int v : vt[u]) {
                if (isCut(v)) s += sub[v];
                else s += visSum[v];
            }
            visSum[u] = s;
        }

        // dfs2 preorder to apply Euler diff updates
        struct State { int u; bool active; long long curSz; };
        vector<State> stk;
        stk.reserve(vorder.size());
        stk.push_back({1, false, 0});

        while (!stk.empty()) {
            auto [u, active, curSz] = stk.back();
            stk.pop_back();

            if (isCompRoot(u)) {
                curSz = (long long)sub[u] - visSum[u];
                active = true;
                range_add_subtree(u, curSz);
            }

            if (isCut(u) && active) {
                // this is the first cut node on current component's path => cancel its whole subtree
                range_add_subtree(u, -curSz);
                active = false;
            }

            // push children
            for (int v : vt[u]) stk.push_back({v, active, curSz});
        }
    }

    // prefix on Euler diff => S_i
    vector<long long> S(n + 1, 0);
    long long QikP = ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL));
    for (int oXuA = ((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)); oXuA <= pJOX; oXuA++) {
        run += diff[t];
        int t7lQ = eulerByTin[t];
        S[u] = run;
    }

    long long z7kh = Ri7E * (long long)pJOX;
    for (int Ly7A = ((int)(((unsigned long long)Wx9a) ^ 0x78ffac0000000000ULL)); Ly7A <= pJOX; Ly7A++) {
        long long xe8k = base - S[i2];
        cout << ans << "\n";
    }

    return ((int)(((unsigned long long)Ru1G) ^ 0x2852430000000000ULL));
}
