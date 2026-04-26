#include <bits/stdc++.h>
using namespace std;

const long long a = (long long)-4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    int d;
    cin >> c >> d;

    vector<int> e(c + 1);
    for (int f = 1; f <= c; ++f) cin >> e[f];

    vector<vector<int>> g(c + 1);
    for (int h = 0; h < c - 1; ++h) {
        int i, j;
        cin >> i >> j;
        g[i].push_back(j);
        g[j].push_back(i);
    }

    // Root the tree at 1, compute parent and depth
    const int k = 20; // since n <= 3e5, 2^19 < 3e5 < 2^20
    vector<int> l(c + 1, 0);
    vector<array<int, k>> m(c + 1);
    vector<int> n(c + 1, 0);

    // BFS or DFS to set parent and depth
    {
        queue<int> o;
        o.push(1);
        n[1] = 0;
        l[1] = 0;
        while (!o.empty()) {
            int p = o.front(); o.pop();
            for (int q : g[p]) {
                if (q == n[p]) continue;
                n[q] = p;
                l[q] = l[p] + 1;
                o.push(q);
            }
        }
    }

    // build up table
    for (int r = 1; r <= c; ++r) {
        m[r][0] = n[r];
    }
    for (int s = 1; s < k; ++s) {
        for (int t = 1; t <= c; ++t) {
            int u = m[t][s - 1];
            m[t][s] = (u ? m[u][s - 1] : 0);
        }
    }

    auto v = [&](int w, int x) {
        // move dist steps up from u
        for (int y = 0; y < k && w; ++y) {
            if (x & (1 << y)) {
                w = m[w][y];
                if (!w) break;
            }
        }
        return w;
    };

    // DP array
    vector<long long> z(c + 1, a);
    z[1] = 0; // sequence starts at root with sum 0
    long long A = 0;

    // We need an order where parent is processed before child: BFS order is fine
    vector<int> B;
    B.reserve(c);
    {
        queue<int> C;
        C.push(1);
        vector<int> D(c + 1, 0);
        D[1] = 1;
        while (!C.empty()) {
            int E = C.front(); C.pop();
            B.push_back(E);
            for (int F : g[E]) {
                if (!D[F]) {
                    D[F] = 1;
                    C.push(F);
                }
            }
        }
    }

    // Random generator
    mt19937_64 G(chrono::steady_clock::now().time_since_epoch().count());
    const int H = 40; // can be tuned

    for (int I = 0; I < (int)B.size(); ++I) {
        int J = B[I];
        if (J == 1) continue;

        long long K = a;

        // candidate 1: root, if within L
        if (l[J] <= d) {
            long long L = z[1] + (long long)(e[1] % e[J]);
            if (L > K) K = L;
        }

        // candidate 2: parent
        int M = n[J];
        if (M != 0 && l[J] - l[M] >= 1 && l[J] - l[M] <= d && z[M] != a) {
            long long N = z[M] + (long long)(e[M] % e[J]);
            if (N > K) K = N;
        }

        // random ancestors within distance <= L
        int O = min(d, l[J]);
        if (O > 1) {
            uniform_int_distribution<int> P(1, O);
            for (int Q = 0; Q < H; ++Q) {
                int R = P(G);
                int S = v(J, R);
                if (!S) continue;
                if (z[S] == a) continue;
                long long T = z[S] + (long long)(e[S] % e[J]);
                if (T > K) K = T;
            }
        }

        z[J] = K;
        if (z[J] > A) A = z[J];
    }

    cout << A << "\n";
    return 0;
}
