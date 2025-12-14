#include <stdio.h>
const int N = 200005;
int a[N];
int main() {
    int n = 0;
    while (~scanf("%d", &a[n++]));
    long long ans = 0;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++)
            if (a[j] > a[i]) ++ans;
    printf("%lld\n", ans);
    return 0;
}