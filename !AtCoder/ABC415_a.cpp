#include <cstdio>
const int N = 105;
int a[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", a + i);
    int x;
    scanf("%d", &x);
    bool flg = false;
    for (int i = 1; i <= n; i++)
        if (a[i] == x) flg = true;
    puts(flg ? "Yes" : "No");
    return 0;
}