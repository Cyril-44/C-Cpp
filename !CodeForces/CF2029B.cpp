#include <stdio.h>
const int N = 100005;
char s[N], r[N];
int main()
{
    int t, n;
    scanf("%d", &t);
    while (t--)
    {
        scanf("%d", &n);
        scanf("%s %s", s, r);
        int a[2] = {0, 0};
        for (int i = 0; i < n; i++)
        {
            ++a[s[i] ^ '0'];
        }
        bool flag = true;
        for (int i = 0; i+1 < n; i++)
        {
            if (!a[0] || !a[1])
                flag = 0;
            --a[r[i] ^ '0' ^ 1];
        }
        puts(flag ? "YES" : "NO");
    }
    return 0;
}