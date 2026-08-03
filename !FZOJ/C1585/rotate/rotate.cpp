#include <cstdio>
#include <map>
#include <algorithm>
constexpr int N = 500005;
std::pair<int, int> a[N], b[N];
inline std::pair<int,int> rev(const std::pair<int,int>& p) {
    return {p.second, p.first};
}
int n;
struct BIT {
    int tr[N];
    void add(int p, int x) {
        for (; p <= n; p += p & -p) tr[p] += x;
    }
    int sum(int p) {
        int res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
} f;
int map[N];
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i].first);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i].second);
    for (int i = 1; i <= n; i++) scanf("%d", &b[i].first);
    for (int i = 1; i <= n; i++) scanf("%d", &b[i].second);
    std::map<std::pair<int,int>, int> _mp;
    for (int i = 1; i <= n; i++) _mp[a[i]] = i;
    for (int i = 1; i <= n; i++) {
        if (_mp.count(b[i])) map[i] = _mp[b[i]];
        else if (_mp.count(rev(b[i]))) map[i] = -_mp[rev(b[i])];
    }
    for (int i = 1; i <= n; i++)
        if ((i - map[i] & 1) != (map[i] < 0)) {
            puts("-1");
            return 0;
        }
    int64_t ans = 0;
    for (int i = n; i >= 1; i--) {
        ans += f.sum(std::abs(map[i]));
        f.add(std::abs(map[i]), 1);
    }
    printf("%lld\n", ans);
    return 0;
}