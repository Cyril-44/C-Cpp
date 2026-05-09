#include <bits/stdc++.h>
using namespace std;

long double collision_prob(long long l, long long m, long long n) {
    if (n > l) return 1.0L;
    if (m <= 0) return 0.0L;
    if (n > l - m) return 1.0L;

    long double no_collision = 1.0L;
    for (long long i = 0; i < n; ++i) {
        no_collision *= (long double)(l - m - i) / (long double)(l);
    }
    return 1.0L - no_collision;
}

int main() {
    long long l, m, n;
    cin >> l >> m >> n;

    long double ans = collision_prob(l, m, n);
    cout << fixed << setprecision(12) << (double)ans << '\n';
    return 0;
}