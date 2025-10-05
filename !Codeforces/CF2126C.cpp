#include <cstdio>
#include <algorithm>
const int N = 100005;
int a[N];
int main() {
    int t, k, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        k = a[k];
        std::sort(a+1, a+1 + n);
        int time = 1;
        int p = std::lower_bound(a+1, a+1+n, k) - a;
        bool flg = true;
        ++k;
        while (p < n) {
            ++p;
            if (a[p] - a[p-1] > k - time) {
                flg = false;
                break;
            }
            k = a[p] + 1;
            time += a[p] - a[p-1];
        }
        puts(flg ? "YES" : "NO");
    }
    return 0;
}