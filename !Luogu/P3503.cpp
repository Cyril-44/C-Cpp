#include <bits/stdc++.h>
constexpr int N = 1000005;
int a[N], sta[N];
int64_t s[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    while (m--) {
        int k; scanf("%d", &k);
        for (int i = 1; i <= n; i++) s[i] = s[i-1] + a[i] - k;
        int ans = 0, top = 0;
        for (int i = 0; i < n; i++)
            if (!top || s[sta[top]] > s[i])
                sta[++top] = i;
        for (int i = n; i >= 1; i--)
            while (top && s[sta[top]] <= s[i])
                ans = std::max(ans, i - sta[top--]);
        printf("%d ", ans);
    }
}