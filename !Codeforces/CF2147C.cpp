#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
char s[N];
int st[N]; // 0->没有兔子 1->1个兔子 -1->多个兔子
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        memset(st, 0, sizeof(int) * (n+1));
        scanf(" %s", s + 1);
        bool flg = true;
        for (int i = 1, cnt = 0; i <= n; i++) {
            if (s[i] ^ '0') cnt = 0;
            else {
                ++cnt;
                if (cnt > 1) st[i] = -1;
                else st[i] = 1;
                if (cnt == 2) st[i-1] = -1;
            }
        }
        if (st[1] == 1) st[1] = -1;
        if (st[n] == 1) st[n] = -1;
        for (int i = 1, j = 1; i <= n; i = ++j) {
            if (st[i] == 1) {
                for (; j < n && ((j-i & 1) ? st[j+1] == 1 : !st[j+1]); j++); // 101010101......
                if (j - i & 1) {
                    int tmp = j - i + 1 >> 1;
                    if ((i <= 2 || !st[i-2]) && (j > n-1 || !st[j+1]) && (tmp & 1)) { flg = false; break; }
                }
            }
        }
        puts(flg ? "Yes" : "No");
    }
    return 0;
}