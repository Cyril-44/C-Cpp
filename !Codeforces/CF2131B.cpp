#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 200005;
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = n/2-1; i > 0; i--) printf("-1 3 ");
        if (n & 1) puts("-1 3 -1");
        else puts("-1 2");
    }
    return 0;
}