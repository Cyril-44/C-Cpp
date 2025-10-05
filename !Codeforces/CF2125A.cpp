#include <cstdio>
const int N = 200005;
char s[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%s", s);
        int tt(0);
        for (n = 0; s[n]; ++n)
            switch (s[n]) {
            case 'T': ++tt; break;
            }
        while (tt--) putchar('T');
        for (int i = 0; i < n; i++)
            if (s[i] ^ 'T') putchar(s[i]);
        putchar('\n');
    }
    return 0;
}