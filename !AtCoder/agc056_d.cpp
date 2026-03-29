#include <cstdio>
#include <cstdint>
#include <algorithm>
constexpr int N = 5005;
int a[N];
int64_t os[N], es[N];
int main() {
    int n;
    int64_t l, r;
    scanf("%d%ld%ld", &n, &l, &r);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::sort(a+1, a+1 + n);
    for (int i = 1; i <= n; i++) {
        os[i] = os[i-1], es[i] = es[i-1];
        (i & 1) ? (os[i] += a[i]) : (es[i] += a[i]);
    }
    for (int alice = 1; alice <= n; alice++) {
        for (int bob = 1; bob <= n; bob++) {
            if (alice == bob) continue;
            int mn = std::min(alice, bob), mx = std::max(alice, bob);
            int64_t av = os[mn-1] + es[mx-1]-es[mn] + os[n]-os[mx] + a[alice],
                    bv = es[mn-1] + os[mx-1]-os[mn] + es[n]-es[mx] + a[bob];
            if (l <= av && av <= r && l <= bv && bv <= r) {
                puts("Alice"); return 0;
            }
        }
    }
    puts("Bob");
    return 0;
}