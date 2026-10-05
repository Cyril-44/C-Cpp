#include <cstdio>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <set>
constexpr int N = 100005;
int a[N];
int main() {
    freopen("mode.in", "r", stdin);
    freopen("mode.out", "w", stdout);
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    int sub = 0;
    int64_t ans = 0;
    std::set<std::pair<int,int>> st;
    for (int i = 1; i <= n; i++) st.emplace(a[i], i);
    for (int i = n; i >= 1; i--) {
        st.erase({a[i], i});
        if (a[i] - sub <= 0) continue;
        int64_t cnt = a[i] - sub;
        for (auto it = st.begin(); it != st.end(); it = st.erase(it)) {
            if (it->first > a[i]) break;
            cnt += it->first - sub;
        }
        cnt += 1ll * st.size() * (a[i] - sub);
        ans += cnt * i;
        sub = a[i];
    }
    printf("%lld\n", ans);

}