#include <bits/stdc++.h>
constexpr int N = 300005;
int v[N], ptr[N];
inline int solve(int L, int R, int mid) {
    memset(ptr, 0, sizeof(int) * (R+2));
    int lastup = mid, lastdown = mid;
    int r;
    for (r = R; r >= L; r--) {
        if (v[r] > mid) {
            if (v[r] > lastup)
                lastup = v[r];
            else break;
        }
        else if (v[r] < mid) {
            if (v[r] < lastdown)
                lastdown = v[r];
            else break;
        }
        else break;
        ptr[v[r]] = r;
    }
    int l; lastup = mid, lastdown = mid;
    int res = 0;
    for (l = L; l <= r; l++) {
        if (v[l] > mid) {
            if (v[l] > lastup)
                lastup = v[l];
            else break;
        }
        else if (v[l] < mid) {
            if (v[l] < lastdown)
                lastdown = v[l];
            else break;
        }
        else break;
        if (ptr[v[l]]) r = std::max(r, ptr[v[l]]);
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
            scanf("%d", &v[i]);
        printf("%d\n", std::max(solve(1, n-1, v[n]), solve(2, n, v[1])));
    }
}