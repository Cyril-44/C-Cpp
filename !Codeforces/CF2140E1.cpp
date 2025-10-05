#include <bits/stdc++.h>
using namespace std;
constexpr int N = 25;
int c[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t, n, m, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &m, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d", &c[i]);
    }
    return 0;
}