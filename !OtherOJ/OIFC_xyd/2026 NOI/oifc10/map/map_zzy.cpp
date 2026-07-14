#include <bits/stdc++.h>
#define ll long long
#define i8 __int128
using namespace std;
template <typename T> void in(T &x) {
    char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (x = 0; isdigit(c); c = getchar()) x = x * 10 + c - '0';
}
const int N = 2005;
int T, n, m, q;
char s[N][N];

struct h {
    ll x, y;
} w[N][N];
i8 dot(h a, h b) {
    return (i8)(a.x) * b.y - (i8)(a.y) * b.x;
}
h operator+(h a, h b) {
    return (h){a.x + b.x, a.y + b.y};
}
h operator-(h a, h b) {
    return (h){a.x - b.x, a.y - b.y};
}

int dis[N][N], col[N][N], c;
vector<h> v[N * N];
const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
struct E {
    int x, y;
    h d;
};
vector<E> g[N][N];

void out(bool o) {
    printf("%s\n", o ? "Bread" : "Map");
}
void dfs(int x, int y, h d, int ds) {
    dis[x][y] = ds, w[x][y] = d, col[x][y] = c;
    for (E now : g[x][y]) {
        int p = now.x, q = now.y;
        if (!dis[p][q])
            dfs(p, q, d + now.d, ds + 1);
        else {
            h z = w[p][q] - w[x][y] - now.d;
            if ((z.x || z.y) && (v[c].empty() || dot(v[c][0], z))) v[c].push_back(z);
        }
    }
}
int main() {
    freopen("map.in", "r", stdin);
    freopen("map.out", "w", stdout);
    in(T);
    while (T--) {
        in(n), in(m);
        for (int i = 0; i < n; ++i) scanf("%s", s[i]);
        for (int x = 0; x < n; ++x)
            for (int y = 0; y < m; ++y)
                if (s[x][y] == '.') {
                    g[x][y].clear(), dis[x][y] = 0;
                    for (int z = 0; z < 4; ++z) {
                        int nx = x + dx[z], ny = y + dy[z];
                        h d = (h){0, 0};
                        if (nx < 0) nx += n, d.x = -1;
                        if (ny < 0) ny += m, d.y = -1;
                        if (nx >= n) nx -= n, d.x = 1;
                        if (ny >= m) ny -= m, d.y = 1;
                        if (s[nx][ny] != '.') continue;
                        g[x][y].push_back((E){nx, ny, d});
                    }
                } else
                    dis[x][y] = -1;
        c = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                if (!dis[i][j]) v[++c].clear(), dfs(i, j, (h){0, 0}, 1);
        in(q);
        ll sx, sy, tx, ty;
        for (int i = 1; i <= q; ++i) {
            in(sx), in(sy), in(tx), in(ty);
            int c = col[sx % n][sy % m];
            if (c != col[tx % n][ty % m])
                out(0);
            else {
                h now = (h){tx / n - sx / n, ty / m - sy / m} - (w[tx % n][ty % m] - w[sx % n][sy % m]);
                if (!now.x && !now.y)
                    out(1);
                else if (v[c].size() != 1)
                    out(!v[c].empty());
                else
                    out(dot(now, v[c][0]) == 0);
            }
        }
    }
    return 0;
}