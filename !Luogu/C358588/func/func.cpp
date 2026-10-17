#include <bits/stdc++.h>
constexpr int N = 2000005;
int a[N], _h[N];
#define h(p) _h[p < N ? p : N-1]
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int n; std::cin >> n;
    for (int i = 1; i <= n; i++) std::cin >> a[i];
    uint64_t ans = 0;
    for (int k = 0; k <= 30; k++) {
        int last1 = 0;
        for (int i = 1; i <= n; i++)
            if (a[i] >> k & 1) {
                ans += (1ull << k) * (i - last1) * (n - i + 1ull);
                last1 = i;
            }
    }
    for (int k = 0; k < 20; k++) {
        memset(_h, 0, sizeof _h);
        const int mex = 1 << k;
        int cnt = 0, lastoccur = 0, l = 1;
        for (int i = 1; i <= n; i++) {
            if (a[i] >> k & 1) lastoccur = i;
            if (a[i] < mex && !h(a[i])) ++cnt;
            ++h(a[i]);
            if (cnt == mex) {
                for (; l <= i && h(mex); l++) {
                    --h(a[l]);
                    if (a[l] < mex && !h(a[l])) --cnt;
                }
                if (cnt == mex) {
                    for (; l <= i && !(a[l] < mex && h(a[l]) == 1); l++) --h(a[l]);
                    if (lastoccur < l) ans += (l - lastoccur) * (1ull << k);
                }
            }
        }
    }
    std::cout << ans << '\n';
}