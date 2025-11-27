#include <bits/stdc++.h>
using namespace std;
int t, n, a[100010];
int main()
{
    scanf("%d", &t);
    while (t--)
    {
        scanf("%d", &n);
        int val = 0;
        for (int i = 1; i <= n; i++)
        {
            scanf("%d", &a[i]);
            bool tg = 0;
            for (int j = 2; j * j <= a[i]; j++)
                if (a[i] % j == 0)
                {
                    tg = 1;
                    if (a[i] & 1)
                        a[i] -= j;
                    break;
                }
            if (tg == 0)
            {
                if (val == 0)
                    val = a[i];
                else
                    val = -1;
            }
        }
        if (val == 0)
            printf("2\n");
        else if (val > 0)
        {
            bool ok = 1;
            for (int i = 1; i <= n; i++)
                if (a[i] != val && a[i] < val * 2)
                    ok = 0;
            if (ok)
                printf("%d\n", val);
            else
                printf("-1\n");
        }
        else
            printf("-1\n");
    }
    return 0;
}