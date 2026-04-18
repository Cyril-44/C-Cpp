#include <bits/stdc++.h>

using namespace std;

const int mod = 998244353;

#define ctz(x) __builtin_popcount(x)
#define ll long long

int n, a[21][1 << 20], b[21][1 << 20];

inline void get(int &x) {
    if (x < 0) x += mod;
    if (x >= mod) x -= mod;
}

inline void dft(int *a) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (1 << n); j++) {
            if (j >> i & 1) get(a[j] += a[j ^ (1 << i)]);
        }
    }
}

inline void idft(int *a) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (1 << n); j++) {
            if (j >> i & 1) get(a[j] -= a[j ^ (1 << i)]);
        }
    }
}

inline ll ksm(ll a, int b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < (1 << n); i++) {
        scanf("%d", &a[ctz(i)][i]);
    }
    for (int i = 0; i <= n; i++) {
        dft(a[i]);
    }
    // for (int i = 0; i < (1 << n); i++) b[0][i] = ksm(a[0][i], mod - 2);
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            for (int k = 0; k < (1 << n); k++) {
                b[i][k] = (b[i][k] + (ll)b[j][k] * a[i - j][k] % mod * j) % mod;
            }
        }
        int iv = ksm(i, mod - 2);
        for (int j = 0; j < (1 << n); j++) b[i][j] = (a[i][j] - (ll)b[i][j] * iv % mod + mod) % mod;
    }
    for (int i = 0; i <= n; i++) {
        idft(b[i]);
    }
    for (int i = 0; i < (1 << n); i++) {
        printf("%d ", b[ctz(i)][i]);
    }
    return 0;
}