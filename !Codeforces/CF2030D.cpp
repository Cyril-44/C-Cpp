#include <stdio.h>
#include <iostream>
using std::min;
using std::max;
const int N = 200005;
int a[N], f[N];
char s[N];
int main() {
    int t, n, q, qi, cnt, id, tmp;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &q);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            f[i] = 0;
        }
        for (int i = 1; i <= n; i++)
            ++f[min(i, a[i])], --f[max(i, a[i])];
        for (int i = 1; i <= n; i++)
            f[i] += f[i - 1];   // fi -> i~i+1需要
        scanf("%s", s);
        cnt = id = 0;
        for (int i = 1; i < n; i++)
            if (s[i] == 'R' && s[i-1] == 'L' && f[i] > 0) {
                ++cnt;
            }
        while (q--) {
            scanf("%d", &qi);   // 字符串中是 qi-1
            if (qi > 1 && f[qi-1] > 0) {
                if (s[qi-2] == 'L' && s[qi-1] == 'R') --cnt;
                else if (s[qi-2] == 'L' && s[qi-1] == 'L') ++cnt;
            }
            if (qi < n && f[qi] > 0) {
                if (s[qi-1] == 'L' && s[qi] == 'R') --cnt;
                else if (s[qi-1] == 'R' && s[qi] == 'R') ++cnt;
            }
            if (s[qi-1] == 'L') s[qi-1] = 'R';
            else s[qi-1] = 'L';
            if (cnt <= 0) puts("YES");
            else puts("NO");
        }
    }
    return 0;
}