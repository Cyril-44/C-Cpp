#include <bits/stdc++.h>
using namespace std;
constexpr int N = 105;
char s[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %s", &n, s+1);
        int one = 0;
        for (int i = 1; i <= n; i++)
            if (s[i] ^ '0') ++one;
        int ans = 0;
        for (int i = n; one--; i--) {
            if (s[i] ^ '1') ++ans;
        }
        printf("%d\n", ans);
    }
    return 0;
}