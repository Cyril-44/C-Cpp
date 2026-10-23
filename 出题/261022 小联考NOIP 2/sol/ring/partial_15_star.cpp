#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    for (int i = 2; i <= n; ++i) {
        int parent;
        cin >> parent;
    }

    // Root selected: one set. Root unselected: an independent set of the
    // cycle on m=n-1 leaves. The latter count is F_{m-1}+F_{m+1}.
    int m = n - 1;
    long long f0 = 0, f1 = 1, fm1 = 0, fmplus1 = 0;
    for (int i = 0; i <= m + 1; ++i) {
        if (i == m - 1) fm1 = f0;
        if (i == m + 1) fmplus1 = f0;
        long long next = (f0 + f1) % MOD;
        f0 = f1;
        f1 = next;
    }
    cout << (1 + fm1 + fmplus1) % MOD << '\n';
    return 0;
}
