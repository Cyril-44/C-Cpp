#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int A[] = {2, 3, 5, 7};
int main() {
    int t;
    scanf("%d", &t);
    long long l, r;
    while (t--) {
        scanf("%lld%lld", &l, &r);
        long long ans = r - l + 1;
        for (int a : A) {
            ans -= r / a - (l-1) / a;
        }
        for (int a : A) for (int b : A) if (a < b)
            ans += (r / (a*b) - (l-1) / (a*b));
        for (int a : A) for (int b : A) for (int c : A) if (a < b && b < c)
            ans -= (r / (a*b*c) - (l-1) / (a*b*c));
        ans += r / (A[0]*A[1]*A[2]*A[3]) - (l-1) / (A[0]*A[1]*A[2]*A[3]);
        printf("%lld\n", ans);
    }
    return 0;
}