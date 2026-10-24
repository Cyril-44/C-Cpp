#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const int N = 100010;
int n, k, a[N], pre[N];
/*
n%2==0 k=0：最大化将原序列划分为 n/2+1 段后的最小段的值
n%2==0 k=1：长度为 n/2+1 的区间和最小值

n%2==1 k=0：枚举删除头或者尾并归约到 n%2==0 k=1
n%2==1 k=1：枚举合并两个相邻的数并归约到 n%2==0 k=0
*/
bool check(int x) {
    int sum = 0, tot = 0;
    for (int i = 1; i <= n; i++) {
        sum += a[i];
        if (sum >= x) sum = 0, tot++;
    }
    return tot >= n / 2 + 1;
}
pii f[N][2];
bool check2(int x) {
    for (int i = 1; i <= n; i++) {
        f[i][0] = f[i][1] = {0, 0};
        for (int j = 0; j < 2; j++) {
            pii tmp = f[i - 1][j];
            tmp.second += a[i];
            if (tmp.second >= x) tmp.second = 0, tmp.first++;
            f[i][j] = max(f[i][j], tmp);
        }
        if (i > 1) {
            pii tmp = f[i - 2][0];
            tmp.second += a[i - 1] + a[i];
            if (tmp.second >= x) tmp.second = 0, tmp.first++;
            f[i][1] = max(f[i][1], tmp);
        }
    }
    return max(f[n][0].first, f[n][1].first) >= n / 2 + 1;
}
int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    if (n == 1) {
        printf("%d", a[1]);
        return 0;
    }
    if (n % 2 == 0) {
        if (k == 0) {
            int l = 1, r = 1000000000, mid;
            while (l <= r) {
                mid = l + r >> 1;
                if (check(mid))
                    l = mid + 1;
                else
                    r = mid - 1;
            }
            printf("%d\n", r);
        } else {
            for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i];
            int ans = INT_MAX;
            for (int i = n / 2 + 1; i <= n; i++) ans = min(ans, pre[i] - pre[i - (n / 2 + 1)]);
            printf("%d\n", ans);
        }
    } else {
        if (k == 0) {
            for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i];
            int ans1 = INT_MAX, ans2 = INT_MAX;
            for (int i = n / 2 + 1; i < n; i++) ans1 = min(ans1, pre[i] - pre[i - (n / 2 + 1)]);
            for (int i = n / 2 + 2; i <= n; i++) ans2 = min(ans2, pre[i] - pre[i - (n / 2 + 1)]);
            printf("%d\n", min(ans1, ans2));
        } else {
            int l = 1, r = 1000000000, mid;
            while (l <= r) {
                mid = l + r >> 1;
                if (check2(mid))
                    l = mid + 1;
                else
                    r = mid - 1;
            }
            printf("%d\n", r);
        }
    }
}