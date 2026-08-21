#include <cstdio>
#include <algorithm>
constexpr int N = 4000005;
struct Node {
    int val, fr;
    bool operator<(const Node &o) const { return val < o.val; }
} a[N];
int h[N];
template <class T = int>
inline T read() {
    char c = getchar();
    bool sign = (c == '-');
    while (c < '0' || c > '9')
        c = getchar(), sign |= (c == '-');
    T x = 0;
    while ('0' <= c && c <= '9')
        x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return sign ? (~x + 1) : x;
}
int main() {
    int n = read(), top = 0;
    for (int i = 1; i <= n; i++) {
        int k = read();
        while (k--) {
            scanf("%d", &a[++top].val);
            a[top].fr = i;
        }
    }
    std::sort(a+1, a+1+top);
    int cnt = 0;
    int ans = 2e9;
    for (int l = 1, r = 1; r <= top; r++) {
        if (h[a[r].fr]++ == 0) ++cnt;
        while (l < r && h[a[l].fr] > 1) --h[a[l++].fr];
        if (cnt == n) ans = std::min(ans, a[r].val - a[l].val);
    }
    printf("%d\n", ans);
}