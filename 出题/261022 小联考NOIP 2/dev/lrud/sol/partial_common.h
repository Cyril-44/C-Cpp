#pragma once
#include <bits/stdc++.h>
using namespace std;

inline string encode(const vector<int>& path, int n) {
    const string names[] = {"QWE", "A?D", "ZXC"};
    string s;
    for (int i = 1; i < (int)path.size(); ++i) {
        int dr = path[i]/n-path[i-1]/n, dc = path[i]%n-path[i-1]%n;
        s += names[dr+1][dc+1];
    }
    return s;
}

// Hamilton-path subset DP; no maximum-value formula is used.
inline string subsetPath(int n) {
    int k = n*n, all = (1<<k)-1;
    vector<vector<int>> adj(k);
    for (int u = 0; u < k; ++u)
        for (int v = 0; v < k; ++v)
            if (u != v && max(abs(u/n-v/n), abs(u%n-v%n)) == 1) adj[u].push_back(v);
    vector<int8_t> dp((1<<k)*k, -1);
    dp[k] = 0; // mask=1, endpoint=0.
    for (int mask = 1; mask <= all; mask += 2)
        for (int u = 0; u < k; ++u) {
            int cur = dp[mask*k+u];
            if (cur < 0) continue;
            for (int v : adj[u]) if (!(mask>>v&1)) {
                int next = mask | (1<<v);
                if (v == k-1 && next != all) continue;
                int add = u/n == v/n && v%n == u%n+1;
                int8_t& cell = dp[next*k+v];
                cell = max<int>(cell, cur+add);
            }
        }
    vector<int> path;
    int mask = all, u = k-1;
    while (u != 0 || mask != 1) {
        path.push_back(u);
        int old = mask ^ (1<<u), previous = -1;
        for (int v : adj[u]) if ((old>>v&1) && dp[old*k+v] >= 0) {
            int add = v/n == u/n && u%n == v%n+1;
            if (dp[old*k+v]+add == dp[mask*k+u]) { previous = v; break; }
        }
        assert(previous != -1);
        mask = old; u = previous;
    }
    path.push_back(0);
    reverse(path.begin(), path.end());
    return encode(path,n);
}
inline string rowSnake(int n) {
    vector<int> p;
    for (int r = 0; r < n; ++r)
        for (int j = 0; j < n; ++j) p.push_back(r*n+((r&1)?n-1-j:j));
    return encode(p,n);
}
inline string columnBands(int n) {
    vector<int> p;
    for (int j = 0; j < n/2; ++j)
        for (int k = 0; k < n; ++k) {
            int r = (j&1)?n-1-k:k;
            p.push_back(r*n+2*j); p.push_back(r*n+2*j+1);
        }
    return encode(p,n);
}
inline void printPath(const string& s) {
    cout << count(s.begin(),s.end(),'D') << '\n' << s << '\n';
}
// Explicit WA outside the documented supported subtask range.
inline void unsupported() { cout << "0\nD\n"; }
