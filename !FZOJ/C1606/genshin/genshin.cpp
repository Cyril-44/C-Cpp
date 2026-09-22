#include <cstdio>
constexpr int N = 200004;
unsigned s[N];
template<class T> inline void in(T &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
int main() {
    int n; in(n);
    unsigned ans = 0;
    for (int i = 1; i <= n; i++)
        in(s[i]), ans ^= s[i], s[i] += s[i-1];
    for (int i = 1; i <= n; i++)
        for (int j = i-1; j; j = (j-1) & (i-1))
            ans ^= s[i] - s[i - j - 1];
    printf("%u\n", ans);
    return 0;
}