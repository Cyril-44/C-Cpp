#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 5000005;
int f[N], sta[N], a[N][3];
char s[N];
int main() {
    int T, n, m, top;
    scanf("%*d%d", &T);
    while (T--) {
        scanf("%s", s+1);
        n = strlen(s+1);

        m = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] == s[i-1]) ++a[m][1];
            else a[++m][0] = s[i], a[m][1] = 1, a[m][2] = i;
        }

        top = 0;
        a[m+1][0] = 114514;
        for (int j = 1; j <= m; j++) {
            int idx = 0, l = 0;
            if (top && a[j-1][0] > a[j][0]) {
                if (a[j][0] == a[sta[top]][0]) {
                    idx = a[sta[top]][1] + a[sta[top]][2] - 2;
                    l = idx - std::min(a[j][1], a[sta[top]][1]);
                }
                if (a[j][0] == a[sta[top]][0] && a[j][1] == a[sta[top]][1]) --top;
                else top = 0;
            }
            if (a[j-1][0] > a[j][0] && a[j][0] < a[j+1][0]) top = 0;
            if (!top || !(a[j][0] > a[j+1][0]))
                sta[++top] = j;
            int mnf = f[a[j][2] - 1];
            for (int i = a[j][2], k = a[j][1] + a[j][2]; i < k; i++) {
                f[i] = mnf + 1;
                if (idx > l) mnf = std::min(mnf, f[i] = std::min(f[i], f[idx--] + 1));
            }
        }
        printf("%d\n", f[n]);
    }
    return 0;
}