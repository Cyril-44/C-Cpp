#include <cstdio>
#include <algorithm>
constexpr int N = 1004;
int f[N][N];
char s[N];
int sb[N], sw[N], su[N];
inline int sum(int* s, int l, int r) {
    return s[r] - s[l-1];
}
int main() {
    int n, xb, xw;
    scanf("%d%d%d %s", &n, &xb, &xw, s+1);
    for (int i = 1; i <= n; i++) {
        sb[i] = sb[i-1] + (s[i] == 'B');
        sw[i] = sw[i-1] + (s[i] == 'W');
    }
    return 0;
}