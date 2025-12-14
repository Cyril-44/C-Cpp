#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
array<int, N> a, b, h;
int main()
{
    ifstream in("paper.in"), out("paper.out");
    int n, prev, now;
    in >> n;
    for (int i = 1; i < n; i++)
        in >> a[i];
    out >> prev;
    ++h[prev];
    for (int i = 1; i < n; i++)
    {
        out >> now;
        b.at(min(now, prev))++;
        b.at(max(now, prev))--;
        prev = now;
        ++h[now];
    }
    for (int i = 1; i <= n; i++)
        b[i] += b[i - 1];
    bool flg = true;
    for (int i = 1; i <= n; i++)
        if (h[i] != 1)
            printf("Not a permutation! %d occured %d times!\n", i, h[i]), flg = false;
    if (flg)
        for (int i = 1; i <= n; i++)
            if (a[i] != b[i])
                printf("Wrong Answer on Index %d: Exp %d, Found %d.\n", i, a[i], b[i]), flg = false;
    if (!flg)
        return 1;
    puts("Accepted!");
    return 0;
}
