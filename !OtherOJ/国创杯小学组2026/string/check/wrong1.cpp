#include <cstdio>
#include <cstring>
#include <array>
constexpr int N = 5000005;
int f[N]; char s[N];
std::array<int,2> sta[N], a[N];
int main() {
    int T, n, m, top, ans;
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s+1);
        n = strlen(s+1);
        
        m = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] == s[i-1]) ++a[m][1];
            else a[++m] = {s[i], 1};
        }

        ans = m, top = 0;
        for (int i = 1; i <= m; i++) {
            if (!top || a[i][0] > a[i-1][0]) { if (!(top && i < m && a[i][0] > a[i-1][0] && a[i][0] > a[i+1][0])) sta[top++] = a[i]; }
            else if (sta[--top] == a[i]) ans -= 2;
            else if (sta[top][0] != a[i][0]) sta[(top=0)++] = a[i];
            else if (sta[top][1] > a[i][1]) --ans, top = 0;
            else --ans, sta[top = 1] = {a[i][0], a[i][1] - sta[top][1]};
        }
        printf("%d\n", ans);
    }
    return 0;
}