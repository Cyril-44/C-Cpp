#include <bits/stdc++.h>
using namespace std;
constexpr int N = 105;
char a[N], b[N], c[N];
int main() {
    int t, n, m;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %s %d %s %s", &n, a + 50, &m, b, c);
        int l = 50, r = 50 + n;
        for (int i = 0; i < m; i++) {
            if (c[i] == 'V') a[--l] = b[i];
            else a[r++] = b[i];
        }
        a[r] = '\0';
        puts(a + l);
    }
    return 0;
}