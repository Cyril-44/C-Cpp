#include <stdio.h>
#include <algorithm>
const int N = 100005;
int a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%d", a+i);
        std::nth_element(a, a + (n>>1), a + n);
        printf("%d\n", a[n>>1]);
    }
    return 0;
}