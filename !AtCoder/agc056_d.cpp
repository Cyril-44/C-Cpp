#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <vector>
constexpr int N = 5005;
int a[N];
int main() {
    int n;
    int64_t l, r, x = 0, ans = 9e18;
    scanf("%d%ld%ld", &n, &l, &r);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), x += a[i];
    x -= l + r;
    std::sort(a+1, a+1 + n);
    for (int i = 1; i <= n; i++) {
        std::vector<int64_t> vec; vec.reserve(n);
        for (int j = 1; j <= n; j++) if (i != j) vec.push_back(a[j]);
        vec.insert(std::lower_bound(vec.begin(), vec.end(), x + a[i]), x + a[i]);
        int64_t sum = 0;
        for (int j = 1; j < n; j += 2) sum += vec[j] - vec[j-1];
        ans = std::min(ans, sum);
    }
    puts(ans <= r - l ? "Alice" : "Bob");
    return 0;
}