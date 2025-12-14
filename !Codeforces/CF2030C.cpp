#include <stdio.h>
const int N = 200005;
char s[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %s", &n, s);
        bool flg = s[0] == '1' || s[n-1] == '1';
        for (int i = 1; !flg && i < n; i++)
            if (s[i] & s[i-1] & 1)
                flg = true;
        puts(flg ? "YES" : "NO");
    }
    return 0;
}