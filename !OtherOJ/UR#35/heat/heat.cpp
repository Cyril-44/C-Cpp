#include <bits/stdc++.h>
constexpr int N = 200005;
char str[N];
int sum[N], f[N];
inline int s0(int i, int j) { return sum[j] - sum[i-1]; }
inline int s1(int i, int j) { return j-i+1 - s0(i, j); }
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %s", &n, str+1);
        memset(f, 0x3f, sizeof(int) * (n+1));
        for (int i = 1; i <= n; i++) sum[i] = sum[i-1] + (str[i] == '0');
        int l1 = 0, r1 = n+1;
        f[1] = 0;
        for (int i = 1, j = n; i <= j; i++, j--) {
            if (str[i] != str[j]) {
                int l=i, r=(n+1)/2;
                while (l <= r) {
                    int mid = l + r >> 1;
                    if (s1(i, mid) && s1(n-mid+1, j)) r = mid - 1;
                    else l = mid + 1;
                }
                // fprintf(stderr, "%d<->%d: %d\n", i, j, l);
                if (l <= (n+1)/2)
                    f[l+1] = std::min(f[l+1], f[i] + s0(i,l) + s0(n-l+1,j));
                else f[l] = std::min(f[l], f[i] + s0(i,j));
                if (l1 != 0 && r1 != n+1)
                    f[i+1] = std::min(f[i+1], f[i] + i-l1 + r1-j - 1);
                l1 = i, r1 = j;
            } else f[i+1] = std::min(f[i+1], f[i]);
            if (str[i] == '1') l1 = i;
            if (str[j] == '1') r1 = j;
        }
        printf("%d\n", f[(n+1) / 2 + 1]);
    }
}