#include <cstdio>
#include <cstring>
#include <algorithm>
using std::max;
using std::min;
constexpr int N = 200005;
char s[N];
int main() {
    int t, n, x;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d %s", &n, &x, s+1);
        int lp = -1, rp = -1;
        for (int i = x; i >= 1; i--)
            if (s[i] == '#') {lp = i; break;}
        for (int i = x; i <= n; i++)
            if (s[i] == '#') {rp = i; break;}
        if (lp == -1 && rp == -1) puts("1");
        else if (lp == -1 || lp < n-rp+1) printf("%d\n", min(x, n-rp+1+1));
        else /* rp == -1 || lp > n-rp+1*/ printf("%d\n", min(n-x+1, lp+1));
    }
    return 0;
}