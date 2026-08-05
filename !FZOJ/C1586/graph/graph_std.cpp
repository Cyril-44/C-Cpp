#include <bits/stdc++.h>
using namespace std;
const int N = 1000010;
std::vector<int> R[N];
int T, p, k, q, u, v, cnt;
int f[N], bel[N], g[325][325];
template <class T>
inline void chkmax(T &x, T y) {
    if (x < y) x = y;
}
template <class T>
inline void chkmin(T &x, T y) {
    if (x > y) x = y;
}
inline void solve(void) {
    cin >> p >> k >> q, cnt = bel[0] = 0;
    for (int i = 1; i < p; i++) {
        f[i] = 1e9, bel[i] = 0;
    }
    if (k == 0 || k == 1) {
        while (q--) {
            cin >> u >> v;
            cout << (u == v ? 0 : -1) << '\n';
        }
        return;
    }
    std::deque<int> Q{0};
    while (!Q.empty()) {
        u = Q.front(), Q.pop_front();
        int s = (u + 1) % p, t = 1ll * u * k % p;
        if (f[t] > f[u]) f[t] = f[u], Q.push_front(t);
        if (f[s] > f[u] + 1) f[s] = f[u] + 1, Q.push_back(s);
    }
    for (int i = 0; i < p; i++) {
        if (bel[i]) continue;
        cnt++, R[cnt].clear();
        for (int j = i; !bel[j]; j = 1ll * j * k % p) {
            R[cnt].push_back(j), bel[j] = cnt;
        }
    }
    const int a = R[1].size();
    if (pow(cnt, 3) <= 1ll * a * q) {
        memset(g, 0x3f, sizeof(g));
        for (int i = 0; i < p; i++) {
            g[bel[i]][bel[(i + 1) % p]] = 1;
        }
        for (int i = 1; i <= cnt; i++) g[i][i] = 0;
        for (int k = 1; k <= cnt; k++) {
            for (int i = 1; i <= cnt; i++) {
                for (int j = 1; j <= cnt; j++) {
                    chkmin(g[i][j], g[i][k] + g[k][j]);
                }
            }
        }
        while (q--) {
            cin >> u >> v;
            cout << g[bel[u]][bel[v]] << '\n';
        }
    } else {
        while (q--) {
            cin >> u >> v;
            int ans = INT_MAX;
            for (auto &x : R[bel[u]]) {
                chkmin(ans, f[(v - x + p) % p]);
            }
            cout << ans << '\n';
        }
    }
}
inline void optimizeIO(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL), cout.tie(NULL);
}
int main(int argc, char const *argv[]) {
    optimizeIO(), cin >> T;
    while (T--) solve();
    return 0;
}
