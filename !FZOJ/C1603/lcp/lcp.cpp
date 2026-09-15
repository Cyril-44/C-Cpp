#include <cstdio>
#include <cstring>
#include <algorithm>
const int N = 5004;
int a[N], border[N], f[N][N], lcp[N], cnt[N];
void umax(int &x, int y) { if (x < y) x = y; }
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++)
        for (lcp[i] = 0; i+lcp[i] <= n && a[lcp[i]+1] == a[i+lcp[i]]; lcp[i]++);
    for (int i = 2, j = 0; i <= n; i++) {
        while (j && a[j+1] != a[i]) j = border[j];
        if (a[j+1] == a[i]) j++;
        border[i] = j;
    }
    for (int i = 1; i <= n; i++)
        for (int j = 2; j <= i - border[i] + 1; j++)
            cnt[i] += std::min(lcp[j], i - j + 1);
    memset(f, -0x3f, sizeof(f));
    f[0][0] = 0;
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j < n; j++) umax(f[i][j + 1], f[i][j] + 1);
        for (int j = 1; i+j <= m && j <= n; j++) umax(f[i + j - border[j]][border[j]], f[i][j] + cnt[j]);
    }
    printf("%d\n", f[m][0]);
}
