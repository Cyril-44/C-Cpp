#include <bits/stdc++.h>
using namespace std;
constexpr int N = 35;
int n, k;
int f[N][N*N];
int extr[N], top;
void dfs(int x, int v) {
    if (x == 0)
        return;
    extr[++top] = x;
    dfs(f[x][v], v - (x - f[x][v]) * (x - f[x][v] - 1) / 2);
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T;
    cin >> T;
    while (T--) {
        cin >> n >> k;
        if (k == 0) {
            for (int i = 1; i <= n; i++)
                cout << i << ' ';
            cout << '\n';
            continue;
        }
        int mx = n * (n-1) / 2;
        memset(f, -1, sizeof f);
        k = mx - k;
        f[0][0] = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++)
                for (int tp = (i - j) * (i - j - 1) / 2, v = tp; v <= mx; v++)
                    if (~f[j][v - tp]) {
                        f[i][v] = j;
                    }
        }
        if (f[n][k] == -1) { cout << "0\n"; continue; }
        top = 0; dfs(n, k);
        reverse(extr+1, extr+1 + top);
        for (int i = 1; i <= top; i++)
            for (int j = extr[i]; j > extr[i-1]; j--)
                cout << j << ' ';
        cout << '\n';
    }
    return 0;
}
