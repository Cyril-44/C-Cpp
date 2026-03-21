#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
template <class T>
void in(T &x) {
    char c = getchar();
    T f = 1;
    x = 0;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 100010, Mod = 1e9 + 7;
int t, n, k;
char str[N], str2[N];
ll mi[N];
int cnt[N];
ll work(char *str) {
    for (int i = 1; i <= n; i++)
        cnt[i] = cnt[i - 1] + (str[i] == '?');
    // for(int i=1;i<=k;i++) putchar(str[i]=='?'?'+':'-'); putchar('\n');
    ll ans = 0;
    int bs = 0;
    bool taga, tagb;
    for (int i = n; i >= 0; i -= k) {
        int add = max(0, i - k + 1);
        add += cnt[min(k, i)];
        // if(i<k) add+=taga+tagb-1;
        add = min(add, i);
        // printf("(%d,%d,%d,%d) ",i,add,(int)taga,(int)tagb);
        (ans += mi[add]) %= Mod;
        // printf("%s\n",str+1);
        if (i >= k) {
            taga = 1, tagb = 1;
            for (int j = i; j > i - k; j--)
                if (str[j] == '0') {
                    if (j - k > 0) {
                        str[j] ^= 1;
                        if (str[j - k] != '?')
                            str[j - k] ^= 1;
                    } else
                        taga = 0;
                }
            for (int j = i; j > i - k; j--)
                if (str[j] == '1') {
                    if (j - k > 0) {
                        str[j] ^= 1;
                        if (str[j - k] != '?')
                            str[j - k] ^= 1;
                    } else
                        tagb = 0;
                }
            if (taga) {
                for (int j = i; j > i - k; j--)
                    if (str[j] == '0') {
                        if (j - k > 0) {
                            str[j] ^= 1;
                            if (str[j - k] != '?')
                                str[j - k] ^= 1;
                        } else
                            taga = 0;
                    }
            } else if (tagb) {
                for (int j = i; j > i - k; j--)
                    if (str[j] == '1') {
                        if (j - k > 0) {
                            str[j] ^= 1;
                            if (str[j - k] != '?')
                                str[j - k] ^= 1;
                        } else
                            tagb = 0;
                    }
            } else
                break;
        }
    }
    return ans;
}
int main() {
    freopen("snake2.in", "r", stdin);
    freopen("snake.out", "w", stdout);
    mi[0] = 1;
    for (int i = 1; i < N; i++)
        mi[i] = (mi[i - 1] << 1) % Mod;
    in(t);
    while (t--) {
        in(n);
        in(k);
        scanf("%s", str + 1);
        for (int i = 1; i <= n; i++) {
            if (str[i] == 'o')
                str[i] = '0';
            else if (str[i] == 'x')
                str[i] = '1';
        }
        for (int i = 1; i <= k; i++) {
            int cur = i;
            for (int j = i; j <= n; j += k) {
                if (str[j] == '?') {
                    swap(str[j], str[cur]);
                    cur += k;
                }
            }
        }
        printf("%lld\n", work(str));
    }
    return 0;
}