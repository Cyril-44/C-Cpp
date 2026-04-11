#include <cstdio>
#include <algorithm>
const int N = 3005;
int a[N];
int main() {
    int n, Mod;
    scanf("%d%d", &n, &Mod);
    a[0] = -1;
    int ans = 0;
    for (int i = 1; i <= n; i++) a[i] = i;
    do {
        int cnt = 0, mx = 0;
        for (int i = 1; i <= n; i++) {
            if (a[i] == a[i - 1] + 1) cnt++;
            else {
                mx = std::max(mx, cnt);
                cnt = 1;
            }
        }
        mx = std::max(mx, cnt);
        ans += mx;
    } while (std::next_permutation(a + 1, a + 1 + n));
    printf("%d\n", ans % Mod);
    return 0;
}