#include <stdio.h>
#include <string.h>
#include <algorithm>
const int N = 200005;
char s[N];
int h[26];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %s", &n, s);
        memset(h, 0, sizeof h);
        for (int i = 0; i < n; i++)
            ++h[s[i] - 'a'];
        bool f = true;
        while (f) {
            f = false;
            for (int i = 0; i < 26; i++)
                if (h[i]) {
                    putchar(i + 'a');
                    --h[i], f = true;
                }
        }
        putchar('\n');
    }
    return 0;
}