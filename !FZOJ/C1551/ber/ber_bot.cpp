#include <bits/stdc++.h>
using namespace std;
const long long MOD = 421969921;

long long mod_pow(long long a, __int128 e) {
    a %= MOD;
    long long r = 1 % MOD;
    while (e > 0) {
        if (e & 1) r = (__int128)r * a % MOD;
        a = (__int128)a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    long long A;
    long long k;
    cin >> n >> A >> k;

    for (long long i = 0; i < n - 1; i++) {
        long long x, y;
        cin >> x >> y; // irrelevant to answer
    }

    long long n2 = (__int128)n * n % MOD;

    if (A == 0) {
        cout << n2 % MOD << "\n";
        return 0;
    }

    __int128 E = (__int128)A * n - 1;              // exponent An-1
    long long km = ((k % MOD) + MOD) % MOD;
    long long base = ((k - 1) % MOD + MOD) % MOD;  // k != 1 mod MOD guaranteed

    long long powpart = mod_pow(base, E);
    long long ans = (__int128)n2 * km % MOD;
    ans = (__int128)ans * powpart % MOD;

    cout << ans << "\n";
}
