#include <bits/stdc++.h>
using namespace std;
int main() {
    int t, n, a, b;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &a, &b);
        if ((n&1) == (b&1) && (n&1) == (std::max(a,b)&1)) puts("YES");
        else puts("NO");
    }
    return 0;
}