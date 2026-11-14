#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<long long> A(N);
    for (auto &x : A) cin >> x;

    vector<vector<int>> graph(N);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<char> visited(N, false);
    vector<long long> answer(N);
    for (int start = 0; start < N; ++start) {
        if (visited[start]) continue;

        vector<int> component;
        stack<int> st;
        st.push(start);
        visited[start] = true;
        long long best = A[start];

        while (!st.empty()) {
            int u = st.top();
            st.pop();
            component.push_back(u);
            best = max(best, A[u]);
            for (int v : graph[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    st.push(v);
                }
            }
        }

        for (int u : component) answer[u] = best;
    }

    for (long long x : answer) cout << x << '\n';
    return 0;
}
