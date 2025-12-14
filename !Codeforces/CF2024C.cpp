#include <stdio.h>
#include <algorithm>
const int N = 100005;
int b[N<<1];
struct Node {
    int a1, a2;
    int l, r, comp;
    inline bool operator<(const Node &x) const {
        return comp < x.comp || comp == x.comp && a1 < x.a1;
    }
} a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d%d", &a[i].a1, &a[i].a2);
            b[i<<1] = a[i].a1, b[i<<1|1] = a[i].a2;
        }
        std::sort(b, b + (n<<1));
        for (int i = 0; i < n; i++) {
            a[i].l = std::lower_bound(b, b + (n<<1), a[i].a1) - b +
                std::lower_bound(b, b + (n<<1), a[i].a2) - b;
            a[i].r = b + (n<<1) - std::upper_bound(b, b + (n<<1), a[i].a1) +
                b + (n<<1) - std::upper_bound(b, b + (n<<1), a[i].a2);
            a[i].comp = a[i].l - a[i].r;
        }
        std::sort(a, a + n);
        for (int i = 0; i < n; i++)
            printf("%d %d ", a[i].a1, a[i].a2);
        putchar('\n');
    }
    return 0;
}