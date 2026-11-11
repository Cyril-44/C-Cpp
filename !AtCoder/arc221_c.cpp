#include <bits/stdc++.h>
constexpr int N = 300005;
int a[N], ptr[N];
inline int solve(int L, int R, int mid) {
    memset(ptr, 0, sizeof(int) * (R+2));
    int lastup = mid, lastdown = mid;
    int r;
    for (r = R; r >= L; r--) {
        if (a[r] > mid) {
            if (a[r] > lastup)
                lastup = a[r];
            else break;
        }
        else if (a[r] < mid) {
            if (a[r] < lastdown)
                lastdown = a[r];
            else break;
        }
        else break;
        ptr[a[r]] = r;
    }
    int l; lastup = mid, lastdown = mid;
    int res = 0;
    for (l = L; l <= r; l++) {
        if (a[l] > mid) {
            if (a[l] > lastup)
                lastup = a[l];
            else break;
        }
        else if (a[l] < mid) {
            if (a[l] < lastdown)
                lastdown = a[l];
            else break;
        }
        else break;
        if (ptr[a[l]]) r = std::max(r, ptr[a[l]]);
        res = std::max(res, l-L+1 + R-r);
    }
    return res + 1;
}
/*
1
5
1 5 2 6 3
*/
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        printf("%d\n", std::max(solve(1, n-1, a[n]), solve(2, n, a[1])));
    }
}