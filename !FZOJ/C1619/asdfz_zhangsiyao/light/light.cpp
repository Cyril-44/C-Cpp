#include <bits/stdc++.h>
constexpr int N = 200005;
int s[N*2];
int main() {
    freopen("light.in", "r", stdin);
    freopen("light.out", "w", stdout);
    int n, k;
    scanf("%d%d", &n, &k);
    int cnt = 0;
    for (int i = 1, ai; i <= n; i++) {
        s[i] += s[i-1];
        scanf("%d", &ai);
        if (s[i]%2 != ai) ++s[i], --s[i + k], ++cnt;
    }
    for (int i = n+1; i <= n+k; i++) {
        s[i] += s[i-1];
        if (s[i]) { puts("-1"); return 0; }
    }
    printf("%d\n", cnt);
}