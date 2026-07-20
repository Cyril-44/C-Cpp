#include <cstdio>
#include <vector>
#include <cstdint>
int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> p(n+1), l(n+1), r(n+1), sta(n+1);
    for (int i = 1; i <= n; i++)
        scanf("%d", &p[i]);
    int top = 0;
    for (int i = 1; i <= n; i++) {
        while (top && p[sta[top]] > p[i]) --top;
        if (!top) l[i] = sta[top+1];
        else l[i] = r[sta[top]], r[sta[top]] = i;
        sta[++top] = i;
    }
    uint64_t x1=0, x2=0;
    for (int i = 1; i <= n; i++) {
        x1 ^= 1ull * i * (l[i] + 1);
        x2 ^= 1ull * i * (r[i] + 1);
    }
    printf("%llu %llu\n", x1, x2);
    return 0;
}
