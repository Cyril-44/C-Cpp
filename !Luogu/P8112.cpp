#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 10000005;
char t[N], s[N];
int pi[N], f[N];
int main() {
    int n, m;
    scanf("%d%d %s %s", &m, &n, t+1, s+1);
    memset(f, 0x3f, sizeof f);
    pi[1] = 0; f[0] = 0;
    for (int i = 2; i <= m; i++) {
        int j = pi[i-1];
        while (j && t[j+1] != t[i]) j = pi[j];
        if (t[j+1] == t[i]) pi[i] = j + 1;
    }
    int pt = 0, ps = 0;
    for (int i = 1; i <= n; i++) {
        while ((pt == m || t[pt+1] != s[i]) && pt) pt = pi[pt];
        if (t[pt + 1] == s[i]) ++pt;
        f[i] = std::min(f[i], f[i - pt] + 1);
        // fprintf(stderr, "%d %d\n", pt, f[i]);
    }
    if (f[n] == 0x3f3f3f3f) puts("Fake");
    else printf("%d\n", f[n]);
    return 0;
}