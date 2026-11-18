#include <bits/stdc++.h>
constexpr int N = 200005;
#ifdef CLANGD
#define N 25
#endif
std::vector<std::pair<int,int>> a[N];
int ans[N];
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) {
        int x; scanf("%d", &x);
        a[i].emplace_back(x, 0);
    }
    for (int i = 1; i <= q; i++) {
        int p, x; scanf("%d%d", &p, &x);
        a[p].emplace_back(x, i);
    }
    for (int i = 1; i <= n; i++) std::sort(a[i].begin(), a[i].end());
    for (int i = n; i >= 1; i--) {
        
    }
}