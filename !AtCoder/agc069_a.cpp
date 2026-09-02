#include <bits/extc++.h>
constexpr int N = 18;
__gnu_pbds::priority_queue<int> q[1<<N];
int64_t f[N];
int r[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0, l; i < (1<<n); i++) {
        scanf("%d%d", &l, &r[i]);
        q[i].push(l);
    }
    for (int w = 0; w+1 < n; w++) {
        for (int i = 0; i < (1<<n); i += 1<<w+1) {
            if (r[i] > r[i + (1<<w)]) std::swap(r[i], r[i + (1<<w)]);
            q[i].join(q[i + (1<<w)]);
            f[i] += f[i + (1<<w)];
            if (q[i].top() > r[i]) f[i] += q[i].top() - r[i], q[i].push(r[i]);
            std::swap(r[i], r[i + (1<<w)]);
        }
    }
    printf("%lld\n", f[0]);
    return 0;
}