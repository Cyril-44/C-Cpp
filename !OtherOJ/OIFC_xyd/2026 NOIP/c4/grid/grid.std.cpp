#include <bits/stdc++.h>
#define N 1010
#define ll long long
using namespace std;
int n, m, x, y, sz;
ll c[N][N], a[N][N], X[N], Y[N];
bitset<N * N> f, mask, f1, f2;
vector<bitset<N * N>> g;
vector<ll> VAL1(1), VAL2(1), T1(1), T2(1);
void add(int x, int y) {
    x %= sz, y %= sz;
    f1 = ((f << x) | (f >> (sz - x))) & mask;
    f2 = ((f << y) | (f >> (sz - y))) & mask;
    f = f1 | f2;
}
bool sw;
void solve() {
    VAL1.resize(1), VAL2.resize(1), T1.resize(1), T2.resize(1);
    g.clear();
    f.reset(), mask.reset();
    x = 0, y = 0;
    for (int i = 1; i <= n; i++) X[i] = 0;
    for (int j = 1; j <= m; j++) Y[j] = 0;
    sz = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (c[i][j] >= c[x][y]) {
                x = i, y = j;
            }
        }
    }
    ll Xsum = 0;
    for (int i = 1; i <= n; i++) X[i] = c[i][y], Xsum += X[i];
    sz = abs(n - m);
    if (!sz) sz = N * N;
    for (int i = 0; i < sz; i++) mask.set(i);
    f.set(0);
    g.push_back(f);
    for (int j = 1; j <= m; j++) {
        bool t1 = 1, t2 = 1;
        ll val1 = X[1] + c[1][j], val2 = X[1] - c[1][j];

        if (val2 < 0) t2 = 0;
        if (j == y) val1 = val2 = 0, t1 = 1, t2 = 0;

        for (int i = 2; i <= n; i++) {
            if (val1 != X[i] + c[i][j] && val1 != X[i] - c[i][j]) t1 = 0;
            if (val2 != X[i] + c[i][j] && val2 != X[i] - c[i][j]) t2 = 0;
        }

        if (t1 && t2) add(val1, val2);
        else if (t1) add(val1, val1);
        else if (t2) add(val2, val2);
        else return;

        g.push_back(f);

        VAL1.push_back(val1);
        VAL2.push_back(val2);
        T1.push_back(t1);
        T2.push_back(t2);
    }

    if (!f.test(Xsum % sz)) return;

    int pos = Xsum % sz;

    for (int j = m; j >= 1; j--) {
        bool t1 = T1[j], t2 = T2[j];
        ll val1 = VAL1[j], val2 = VAL2[j];

        if (t1) {
            if (g[j - 1][(pos + sz - val1 % sz) % sz]) {
                pos = (pos + sz - val1 % sz) % sz;
                Y[j] = val1;
                continue;
            }
        }

        if (t2) {
            if (g[j - 1][(pos + sz - val2 % sz) % sz]) {
                pos = (pos + sz - val2 % sz) % sz;
                Y[j] = val2;
                continue;
            }
        }

        return;
    }

    ll Ysum = 0;
    for (int j = 1; j <= m; j++) Ysum += Y[j];
    
    ll delta = (n == m) ? 0 : ((Xsum - Ysum) / (m - n));

    for (int i = 1; i <= n; i++) {
        X[i] += delta;
    }

    for (int j = 1; j <= m; j++) {
        Y[j] += delta;
    }

    Xsum += delta * n, Ysum += delta * m;

    if (Xsum != Ysum) return;

    for (int i = 1; i < n; i++) a[i][m] = X[i], a[n][m] -= X[i];
    for (int j = 1; j < m; j++) a[n][j] = Y[j];
    a[n][m] += Y[m];
    
    if (!sw) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                printf("%lld ", a[i][j]);
            }
            printf("\n");
        }
    } else {
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                printf("%lld ", a[j][i]);
            }
            printf("\n");
        }
    }
    exit(0);
}
int main() {
	freopen("grid.in", "r", stdin);
    freopen("grid.out", "w", stdout);
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%lld", &c[i][j]);
        }
    }
    for (int _ = 1; _ <= 2; _++) {
        solve();
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                a[j][i] = c[i][j];
            }
        }
        swap(n, m);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                c[i][j] = a[i][j];
                a[i][j] = 0;
            }
        }
        sw = 1;
    }
}