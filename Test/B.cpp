#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <class T> void in(T &x) {
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
const int N = 10000000;
bool np[N + 10];
int p[N + 10], cnt;
ll f[N + 10], g[N + 10];
void getprime() {
    np[0] = np[1] = 1;
    f[1] = 1;
    g[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!np[i]) {
            f[i] = i + 1;
            g[i] = 1;
            p[++cnt] = i;
        }
        int tmp;
        for (int j = 1; j <= cnt && i * p[j] <= N; j++) {
            np[tmp = i * p[j]] = 1;
            if (i % p[j]) {
                f[tmp] = f[i] * (p[j] + 1);
                g[tmp] = f[i];
            } else {
                f[tmp] = f[i] * p[j] + g[i];
                g[tmp] = g[i];
                break;
            }
        }
    }
}
ll ans[N + 10];
int t, n;
int main() {
    //    freopen(".in","r",stdin);
    //    freopen(".out","w",stdout);
    getprime();
    for (int i = 1; i <= N; i++)
        ans[i] = ans[i - 1] + i - 1 + i - f[i];
    //    in(t);
    //    while(t--){
    //        in(n);
    //        printf("%lld\n",ans[n]);
    //    }
    return 0;
}