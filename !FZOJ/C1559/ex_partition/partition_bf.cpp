#include <stdio.h>
#include <string.h>
#include <vector>
constexpr int N = (int)1e6 + 5;
char s[N];
namespace FastO {
char obuf[1 << 20], *p1 = obuf; const char *p2 = obuf + sizeof obuf;
inline void out(char c) {
    *p1++ = c;
    if (p1 == p2) fwrite(obuf, sizeof obuf, 1, stdout), p1 = obuf;
}
struct Cleaner { ~Cleaner() { fwrite(obuf, p1 - obuf, 1, stdout); } } cleaner;
inline void out(int x) {
    static unsigned char sta[10];
    sta[sta[0] = 1] = x % 10;
    while (x /= 10) sta[++sta[0]] = x % 10;
    while (sta[0]) out(char(sta[sta[0]--] | '0'));
}
inline void out() { out('-'); out('1'); }
} using FastO::out;
int a[N], n;
int ans[N];
inline int calc(int l) {
    if (~ans[l]) return ans[l];
    static int que[2][N], f[N];
    int hd[2]{}, tl[2]{};
#define empty() (hd[type] == tl[type]) 
#define front() que[type][hd[type]]
#define back() que[type][tl[type] - 1]
#define push(val) (que[type][tl[type]++] = val)
#define pop_front() (hd[type]++)
#define pop_back() (tl[type]--)
    int type = 0; push(0);
    for (int i = 1; i <= n; i++) {
        for (type = 0; type ^ 2; ++type)
            while (!empty() && front() + l < i) pop_front();
        type = a[i] ^ 1;
        f[i] = empty() ? n + 1 : f[front()] + 1;
        // fprintf(stderr, "At n=%2d, l=%2d, i=%2d <== %2d, val=%d\n", n, l, i, empty() ? -1 : front(), f[i]);
        type = a[i];
        while (!empty() && f[back()] >= f[i]) pop_back();
        push(i);
    }
#undef empty
#undef front
#undef back
#undef push
#undef pop_front
#undef pop_back
    return ans[l] = f[n];
}
int main() {
    int T;
    scanf("%d", &T);
    for (int tk = 1; tk <= T; tk++) {
        scanf("%s", s + 1);
        n = strlen(s + 1);
        for (int i = 1; i <= n; i++)
            a[i] = a[i-1] ^ (s[i] & 1);
        memset(ans + 1, -1, sizeof(int) * n);
        for (int l = 1; l <= n; l++) {
            int tmp = calc(l);
            int bl = l;
            for (int k = 20; k >= 0; k--)
                if (l + (1 << k) <= n && calc(l + (1 << k)) == tmp)
                    l += 1 << k;
            for (int i = bl; i <= l; i++) ans[i] = tmp;
        }
        for (int i = 1; i <= n; i++)
            (ans[i] > n ? out() : out(ans[i])), out(' ');
        out('\n');
    }
    return 0;
}