#include <stdio.h>
#include <algorithm>
constexpr int N = 5005;
int a[N];
int main() {
    int Tid, T, n, m;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        std::sort(a+1, a+1 + n);
        for (int i = 1; i <= n; i++)
            for (int j = i+1; j <= n; j++) {
                // 找方案使得 存在 k ，ak/2 性价比紧跟在 ai 之后，ak/2 ~ aj 之间的所有 wi=2，且 ai+aj < ak
            }
    }
    return 0;
}