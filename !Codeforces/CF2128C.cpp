#include <cstdio>
const int N = 200005;
int a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        int mx(0), mn((int)1e9);
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) scanf("%d", a + i);
        bool flg = true;
        for (int i = 1; i <= n; i++) {
            if (a[i] / 2  >= mn) { flg = false; break; }
            if (a[i] > mx) mx = a[i];
            if (a[i] < mn) mn = a[i];
        }
        puts(flg ? "YES" : "NO");
    }
    return 0;
}