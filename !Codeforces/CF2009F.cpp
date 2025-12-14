#include <stdio.h>
typedef long long LL;
const int N = 200005;
int a[N];
long long s[N];
inline LL max(const LL &x, const LL &y)
{
    return x > y ? x : y;
}
inline LL min(const LL &x, const LL &y)
{
    return x < y ? x : y;
}
int main()
{
    int t, n, q;
    long long l, r, c, d, x, y;
    long long ans = 0;
    scanf("%d", &t);
    while (t--)
    {
        scanf("%d%d", &n, &q);
        for (int i = 1; i <= n; i++)
        {
            scanf("%d", &a[i]);
            s[i] = s[i - 1] + a[i];
        }
        while (q--)
        {
            scanf("%lld%lld", &l, &r);
            x = (l - 1) / n;
            y = (r - 1) / n;
            c = (l - 1) % n;
            d = (r - 1) % n;
            if (x == y)
            {
                ans = s[min(d + 1 + x, n)] - s[min(x + c, n)];
                ans += s[max(d + 1 + x - n, 0)] - s[max(x + c - n, 0)];
            }
            else
            {
                ans = (y - x - 1) * s[n];
                ans += s[n] - s[min(c + x, n)];
                ans += s[x] - s[max(c + x - n, 0)];
                ans += s[min(d + 1 + y, n)] - s[y];
                ans += s[max(d + 1 + y - n, 0)] - s[0];
            }
            printf("%lld\n", ans);
        }
    }
    return 0;
}