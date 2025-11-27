#include <cstdio>
constexpr int N = 200005;
char s[N];
int main() {
    int n;
    scanf("%d %s", &n, s+1);
    int mem[2]{};
    long long ans = 0;
    int bit0 = 0;
    for (int i = 1; i <= n; i++) {
        if (s[i] & 1 ^ 1) bit0 ^= 1;
        if (!bit0) ++ans;
        ans += mem[bit0];
        ++mem[bit0];
    }
    printf("%lld\n", ans);
    return 0;
}