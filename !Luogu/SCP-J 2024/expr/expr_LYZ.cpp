#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
const int N = 1e5;
int m, q, n, num[2 * N + 5];
char a[22 * N + 5];
int work(int &p)
{
    int x = 0;
    while (a[p] >= '0' && a[p] <= '9')
    {
        x = x * 10 + a[p] - '0';
        p++;
    }
    return x;
}
int dfs(int p, int l, int r)
{
    if (p > n)
        return p;
    bool pd = (p == 173457);
    if (a[p] == 'x')
    {
        bool op = (a[++p] == '<' ? 0 : 1);
        int s = work(++p), p2;
        ++p;
        if (!op)
        {
            p2 = dfs(p, l, min(r, s - 1));
            p2 = dfs(p2, max(l, s), r);
        }
        else
        {
            p2 = dfs(p, max(l, s + 1), r);
            p2 = dfs(p2, l, min(r, s));
        }
    }
    else
    {
        int s = work(p);
        for (int i = l; i <= r; i++)
            num[i] = s;
        return p + 1;
    }
}

int main()
{
    freopen("expr4.in", "r", stdin);
    scanf("%d%d", &m, &q);
    scanf("%s", a + 1);
    n = strlen(a + 1);
    dfs(1, 0, m + 2);
    while (q--)
    {
        int p;
        scanf("%d", &p);
        if (p > m)
            p = m + 2;
        printf("%d\n", num[p]);
    }
}