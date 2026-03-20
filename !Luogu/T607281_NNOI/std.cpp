#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> t(n + 1);
    vector<ll> s(n + 1);
    for (int i = 1; i <= n; ++i) cin >> t[i] >> s[i];

    // Root the tree at 1, build parent and children, and a postorder.
    vector<int> parent(n + 1, 0);
    vector<vector<int>> children(n + 1);
    vector<int> order;
    order.reserve(n);

    {
        vector<int> st;
        st.push_back(1);
        parent[1] = -1;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            order.push_back(v);
            for (int to : g[v]) {
                if (to == parent[v]) continue;
                parent[to] = v;
                children[v].push_back(to);
                st.push_back(to);
            }
        }
    }

    // postorder: process children before parent
    reverse(order.begin(), order.end());

    vector<ll> need(n + 1), avail(n + 1);

    auto feasible = [&](int T) -> bool {
        for (int i = 1; i <= n; ++i)
            avail[i] = (t[i] <= T ? s[i] : 0);

        for (int v : order) {
            ll sumChild = 0;
            for (int u : children[v]) {
                if (need[u] > 0) sumChild += need[u] + 1; // send need[u] + open edge
            }
            ll req = 1 + sumChild - avail[v]; // 1 for v itself
            if (req < 0) req = 0;
            need[v] = req;
        }
        return need[1] == 0;
    };

    int L = 1, R = n, ans = n;
    while (L <= R) {
        int mid = (L + R) >> 1;
        if (feasible(mid)) {
            ans = mid;
            R = mid - 1;
        } else {
            L = mid + 1;
        }
    }

    cout << ans << '\n';
    return 0;
}
