#include <stdio.h>
#include <queue>
#include <algorithm>
constexpr int N = 100005;
int a[N];
int main() {
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        std::sort(a+1, a+1 + n);
        auto check = [&] (int mid) {
            int ptr = std::lower_bound(a+1, a+1 + n, mid) - a;
            int right = 0;
            for (int i = ptr; i <= n; i++) {
                right += 31 - __builtin_clz(a[i] / mid);
            }
            right = std::min(right, m);
            right += n - ptr + 1;
            if (right >= (n + m + 1 >> 1)) return true;
            return false;            
        };
        int l = 1, r = 1e9;
        while (l <= r) {
            int mid = l + r >> 1;
            if (check(mid)) l = mid + 1;
            else r = mid - 1;
        }
        printf("%d\n", r);
        /* std::priority_queue<int> pq;
        for (int i = 1; i <= n; i++) pq.push(a[i]);
        for (int i = 1; i <= m; i++) {
            int tp = pq.top(); pq.pop();
            pq.push(tp >> 1);
            pq.push(tp - (tp >> 1));
        }
        std::vector<int> b;
        b.reserve(n + m);
        while (!pq.empty()) b.push_back(pq.top()), pq.pop();
        printf("%d\n", b[n + m >> 1]); */
    }
    return 0;
}