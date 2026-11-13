#include <bits/stdc++.h>
constexpr int N = 1005;
bool f[N][N];
inline __int128 C3(int x) {
    return x < 3 ? 0 : (__int128)x * (x-1) * (x-2) / 6;
}
void out(__int128 x) {
    int8_t sta[40];
    sta[sta[0] = 1] = x % 10;
    while (x /= 10) sta[++sta[0]] = x % 10;
    while (sta[0]) putchar(sta[sta[0]--] | '0');
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    __int128 ans = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            ans += 1LL * (n-i+1) * (m-j+1) * (std::__gcd(i, j) - 1);
    out(C3((n+1)*(m+1)) - ans*2 - (n+1) * C3(m+1) - (m+1) * C3(n+1));
}