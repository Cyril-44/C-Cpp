#include <bits/stdc++.h>
constexpr int N = 200005;
int b[N], w[N];
int main() {
    int T, n, k;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &k);
        memset(w, 0, sizeof(int) * k);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &b[i]);
            if (b[i] <= k) ++w[((b[i] - i) % k + k) % k];
        }
        printf("%d\n", n - *std::max_element(w, w+k));
    }
}