#include <stdio.h>
// #include <algorithm>
// #include <queue>
// typedef long long LL;
const int N = 200005;
int n;
long long a[N], k;
/* inline bool check(const int &x) {
    std::priority_queue<LL, std::vector<LL>, std::greater<LL>> q;
    for (int i = 0; i < x; i++)
        q.push(a[i]);
    long long tmp;
    for (int j = x; j < n; j++) {
        tmp = q.top(), q.pop();
        q.push(tmp + a[j]);
    }
    static long long res[N];
    for (int i = x; i > 0; i--)
        res[i] = q.top(), q.pop();
    long long sub = 0;
    for (int i = 2; i <= x; i++)
        sub += res[1] - res[i];
    return sub <= k;
} */
int main() {
    int t, ans;
    long long amx, sum;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %lld", &n, &k);
        amx = sum = 0;
        for (int i = 0; i < n; i++) {
            scanf("%lld", &a[i]);
            if (a[i] > amx) amx = a[i];
            sum += a[i];
        }
        for (ans = n; ans >= 1; ans--) {
            if ((sum+k) / ans * ans >= sum && amx <= (sum+k) / ans)
                break;
        }
        printf("%d\n", ans);
    }
    return 0;
}