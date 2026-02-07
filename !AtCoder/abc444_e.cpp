#include <stdio.h>
#include <set>
#include <algorithm>
constexpr int N = 400005;
int a[N];
int main() {
    int n, d;
    scanf("%d%d", &n, &d);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::multiset<int> st;
    auto check = [&] (auto it) {
        if (it != st.begin() && *it - *std::prev(it) < d) return false;
        if (std::next(it) != st.end() && *std::next(it) - *it < d) return false;
        return true;
    };
    long long ans = 0;
    for (int l = 1, r = 1; r <= n; r++) {
        auto it = st.insert(a[r]);
        while (!check(it)) st.erase(st.find(a[l++]));
        ans += r - l + 1;
    }
    printf("%lld\n", ans);
    return 0;
}