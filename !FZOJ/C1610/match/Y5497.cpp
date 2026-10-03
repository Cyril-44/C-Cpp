#include <bits/stdc++.h>
using namespace std;
int n, m, f[1005][1005];
vector<int> v[26];
char s[1005], t[1000005];
int main() {
    scanf("%d%d%s%s", &n, &m, s + 1, t + 1);
    for (int i = 1; i <= m; i++) { v[t[i] - 'A'].push_back(i); }
    memset(f, 0x3f, sizeof(f));
    f[0][0] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= i; j++) {
            f[i][j] = f[i - 1][j];
            if (j) {
                int x = s[i] - 'A';
                int w = upper_bound(v[x].begin(), v[x].end(), f[i - 1][j - 1]) - v[x].begin();
                if (w != v[x].size()) f[i][j] = min(f[i][j], v[x][w]);
            }
        }
    }
    for (int i = n; i >= 0; i--) {
        if (f[n][i] <= m) {
            cout << i << endl;
            return 0;
        }
    }
    return 0;
}