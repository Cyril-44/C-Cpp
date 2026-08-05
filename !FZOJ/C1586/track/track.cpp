#include <bits/stdc++.h>
constexpr int N = 105;
int r[N], n;
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        bool allsame = true;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &r[i]);
            if (r[i] != r[1]) allsame = false;
        }
        if (allsame) puts(n % 4 == 0 ? "Yes" : "No");
        else puts("Yes");
    }
    return 0;
}