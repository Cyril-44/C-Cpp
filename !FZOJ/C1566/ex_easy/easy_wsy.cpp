#include <bits/stdc++.h>

using namespace std;

const int N = 305;
const int mod = 1000000007;

int n, m, x, y;
int b[N << 1], f[N][N][N][2], su[N][N][2], sv[N][N][2];
vector<int> e[N];

inline void add(int &x, int y) {
    x += y;
    if (x >= mod) x -= mod;
}

void dfs(int u) {
    for (int v : e[u]) {
        dfs(v);
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= m; j++) {
                for (int x = 0; x < 2; x++) {
                    su[i][j][x] = su[i - 1][j][x] + f[u][i][j][x];
                    sv[i][j][x] = sv[i - 1][j][x] + f[v][i][j][x];
                }
            }
        }
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= m; j++) {
                su[i][j][1] += su[i][j + 1][1];
                sv[i][j][1] += sv[i][j + 1][1];
            }
        }
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= m; j++) {
                f[u][i][j][0] = f[u][i][j][0] * (sv[i][j][0] + sv[i][j][1]) + f[v][i][j][0] * (su[i - 1][j][0] + su[i][j][1]);
                f[u][i][j][1] = f[u][i][j][1] * sv[i][j][1] + f[v][i][j][1] * su[i][j][1] - f[u][i][j][1] * f[v][i][j][1];
            }
        }
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m; j++) {
            for (int x = 0; x < 2; x++) {
                su[i][j][x] = su[i - 1][j][x] + f[u][i][j][x];
            }
        }
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m; j++) {
            if (i < j) {
                f[u][i][j][0] = su[i - 1][j][0] + f[u][i][j][0] * i;
            } else {
                f[u][i][j][1] += f[u][i][j][0];
            }
        }
    }
}

int main() {
    scanf("%d%d%d%d", &n, &m, &x, &y);
    for (int i = 1; i <= 2 * m; i++) {
        scanf("%d", &b[i]);
    }
    for (int i = 2; i <= n; i++) {
        int j;
        scanf("%d", &j);
        e[j].push_back(i);
    }
    dfs(1);
    int ans = 0;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m; j++) {
            add(ans, f[1][i][j][1]);
        }
    }
    printf("%d\n", ans);
    return 0;
}
