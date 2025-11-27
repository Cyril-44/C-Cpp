#include <cstdio>
int mx;
inline void build(int u, int l, int r) {
    if (l == r) mx = mx > u ? mx : u;
    else build(u<<1, l, l+r>>1), build(u<<1|1, 1+(l+r>>1), r);
}
int main() {
    freopen("P6025_implement.txt", "w", stdout);
    int ans = 0;
    for (int i = 1; i <= 1000; i++) {
        mx = 0;
        build(1, 1, i);
        ans ^= mx;
        printf("%d\t%d\n", mx, ans);
    }
    return 0;
}