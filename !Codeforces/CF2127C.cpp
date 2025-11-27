#include <cstdio>
#include <algorithm>
#include <cstring>
#include <set>
constexpr int N = 200005;
int a[N], b[N];
struct Node {
    int a, b;
    inline bool operator<(const Node &rhs) const {
        return a+b < rhs.a + rhs.b;
    }
} c[N];
int main() {
    int t, n, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++) scanf("%d", a+i);
        for (int i = 1; i <= n; i++) scanf("%d", b+i);
        long long ans = 0;
        for (int i = 1; i <= n; i++) {
            if (a[i] < b[i]) std::swap(a[i], b[i]);
            ans += a[i] - b[i];
            c[i] = {a[i], b[i]};
        }
        int mn = 0x7fffffff;
        std::sort(c+1, c+1 + n);
        std::set<int> st;
        st.insert(c[1].a);
        for (int i = 2; i <= n; i++) {
            auto it = st.lower_bound(c[i].b);
            if (it != st.begin()) --it, mn = std::min(mn, 2*c[i].b - 2*(*it));
            else mn = 0;
            st.insert(c[i].a);
        }
        printf("%lld\n", ans + mn);
    }
    return 0;
}