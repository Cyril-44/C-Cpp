#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <typename T> void in(T &x) {
    x = 0;
    char c = getchar();
    int f = 1;
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
const int N = 500010;
int n, q;
ll d, a[N], b[N], x;
int main() {
    freopen("queue.in", "r", stdin);
    freopen("queue.out", "w", stdout);
    in(n);
    for (int i = 1; i <= n; i++)
        in(a[i]);
    sort(a + 1, a + n + 1);
    for (int i = 1; i <= n; i++)
        b[i] = a[i] - i + 1;
    b[n + 1] = 2e18;
    in(q);
    in(d);
    while (q--) {
        in(x);
        if (x < b[1]) {
            printf("%lld\n", x);
            continue;
        }
        ll rem = d, bkid, len;
        while (rem > 0) {
            bkid = upper_bound(b + 1, b + n + 1, x) - b - 1;
            len = b[bkid + 1] - x;
            if ((len + bkid - 1) / bkid <= rem) {
                x += (len + bkid - 1) / bkid * bkid;
                rem -= (len + bkid - 1) / bkid;
            } else {
                x += bkid * rem;
                rem = 0;
            }
        }
        printf("%lld\n", x);
    }
}