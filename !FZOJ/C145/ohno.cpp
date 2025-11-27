#include <bits/stdc++.h>
using namespace std;
class IO {
    char ibuf[1 << 16], obuf[1 << 16], *ipl = ibuf, *ipr = ibuf, *op = obuf;

public:
    ~IO() { write(); }
    void read() {
        if (ipl == ipr)
            ipr = (ipl = ibuf) + fread(ibuf, 1, 1 << 15, stdin);
    }
    void write() { fwrite(obuf, 1, op - obuf, stdout), op = obuf; }
    char getchar() { return (read(), ipl != ipr) ? *ipl++ : EOF; }
    void putchar(char c) {
        *op++ = c;
        if (op - obuf > (1 << 15))
            write();
    }
    template <typename V>
    IO& operator>>(V& v) {
        int s = 1;
        char c = getchar();
        v = 0;
        for (; !isdigit(c); c = getchar())
            if (c == '-')
                s = -s;
        for (; isdigit(c); c = getchar())
            v = (v << 1) + (v << 3) + (c ^ 48);
        return v *= s, *this;
    }
    IO& operator<<(char c) { return putchar(c), *this; }
    template <typename V>
    IO& operator<<(V v) {
        if (!v)
            putchar('0');
        if (v < 0)
            putchar('-'), v = -v;
        char stk[100], *top = stk;
        for (; v; v /= 10)
            *++top = v % 10 + '0';
        while (top != stk)
            putchar(*top--);
        return *this;
    }
} io;
const int N = 10000010;
int n, k, a[N];
int qzha[N], qzhb[N], zero[N], q[N];
int main() {
    freopen("ohno.in", "r", stdin);
    freopen("ohno.out", "w", stdout);
    io >> n;
    io >> k;
    for (int i = 1; i <= n; i++)
        io >> a[i];
    for (int i = 1; i <= n; i++) {
        qzha[i] = qzha[i - 1] + (a[i] == -1 ? -1 : 1);
        qzhb[i] = qzhb[i - 1] + (a[i] == 1 ? 1 : -1);
        zero[i] = zero[i - 1] + (a[i] == 0);
        // printf("%d %d %d\n",qzha[i],qzhb[i],zero[i]);
    }
    int lcur = 0;
    int l = 1, r = 1;
    q[1] = 0;
    int mn = 0;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        while (zero[lcur] + k < zero[i]) {
            if (q[l] == lcur)
                l++;
            mn = min(mn, qzhb[lcur]);
            lcur++;
        }
        while (l <= r && qzha[q[r]] > qzha[i])
            r--;
        q[++r] = i;
        // printf("*   %d %d\n",qzha[i],qzha[q[l]]);
        ans = max(ans, qzha[i] - qzha[q[l]]);
        if (zero[i] > k) {
            ans = max(ans, qzha[i] - (zero[i] - k) * 2 - mn);
            // printf("**  %d %d\n",qzha[i]-(zero[i]-k)*2,mn);
        }
    }
    printf("%d\n", ans);
}