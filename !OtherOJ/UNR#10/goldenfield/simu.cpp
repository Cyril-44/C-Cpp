#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 11;
int a[N], b[N], c[N];
inline int get(int l, int r) {
    memcpy(b, a, sizeof a);
    while (true) {
        bool brk = true;
        memcpy(c, b, sizeof b);
        for (int i = l+1; i < r; i++)
            if (b[i] < b[i-1] && b[i] < b[i+1])
                c[i] = std::max(b[i-1], b[i+1]), brk = false;
        if (brk) break;
        memcpy(b, c, sizeof c);
    }
    int sum = 0;
    for (int i = l; i <= r; i++) sum += b[i];
    return sum;
}
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    while (q--) {
        int l, r; scanf("%d%d", &l, &r);
        printf("%d\n", get(l, r));
    }
    return 0;
}