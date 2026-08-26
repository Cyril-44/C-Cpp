#include <bits/stdc++.h>
using namespace std;
int n, t[400005], nw;
struct BIT {
    int c[400005];
    int lowbit(int x) { return x & (-x); }
    void update(int x, int k) {
        if (!x) return;
        for (int i = x; i <= n; i += lowbit(i)) { c[i] += k; }
    }
    int query(int x) {
        int cnt = 0;
        for (int i = x; i >= 1; i -= lowbit(i)) { cnt += c[i]; }
        return cnt;
    }
} bit;
bool check(int x) {
    int sum = bit.query(n), cnt0 = sum - bit.query(x - 1);
    int ned = 1, base = 1;
    for (int i = x - 1; i >= 2; i--) {
        if (t[i] >= ned)
            cnt0 += t[i] - ned;
        else
            ned += ned - t[i];
        if (ned > sum) return 0;
    }
    if (cnt0 + t[0] + t[1] >= ned * 2) return 1;
    return 0;
}
int main() {
    cin >> n;
    nw = 1;
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        t[min(x, n)]++;
        bit.update(min(x, n), 1);
        if (i == 1) {
            printf("%d ", max(1, x));
            continue;
        }
        while (nw <= n && check(nw + 1)) nw++;
        printf("%d ", nw);
    }
    printf("\n");
    return 0;
}