#include <cstdio>
#include <algorithm>
#include <cstring>
int main() {
    int t, n, m;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &m);
        if (n >= 2 && m >= 2 && !(n==2&&m==2)) puts("YES");
        else puts("NO");
    }
    return 0;
}