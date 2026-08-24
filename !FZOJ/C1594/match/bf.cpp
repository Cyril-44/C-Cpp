
#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 100005;
int l[N], r[N], f[N];
inline void bf(int n) {
    f[0] = 0;
    for (int i = 1; i <= n; i++) {
        int mnf = *std::max_element(f, f+l[i]);
        for (int j = l[i]; j <= r[i]; j++) {
            int tmp = f[j];
            f[j] = std::max(f[j], mnf + 1);
            mnf = std::max(mnf, tmp);
        }
    }
    printf("%d\n", *std::max_element(f, f+1+n));
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &l[i], &r[i]);
    if (n <= 5000) bf(n);
    return 0;
}