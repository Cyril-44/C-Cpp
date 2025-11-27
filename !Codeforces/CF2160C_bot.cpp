#include <bits/stdc++.h>
using namespace std;

bool check(long long seg, int mm) {
    if (mm == 0) return true;
    if (mm < 1) return false;
    if (seg >= (1LL << mm)) return false;
    int half = (mm - 1) / 2;
    for (int j = 0; j <= half; j++) {
        int bit_j = (seg >> j) & 1;
        int inv = mm - 1 - j;
        int bit_inv = (seg >> inv) & 1;
        if (bit_j != bit_inv)
            return false;
    }
    if (mm % 2 == 1) {
        int mid = (mm - 1) / 2;
        if ((seg >> mid) & 1) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    cin >> T;
    while (T--) {
        long long n;
        cin >> n;
        if (n == 0) { cout << "YES\n"; continue; }
        bool flg = false;
        for (int tt = 0; tt <= 30; tt++) {
            if (flg)
                break;
            if (tt == 0 && (n & 1) != 0) continue;
            if (tt > 0 && (n & 1) == 0) continue;
            long long low_t = n & ((1LL << tt) - 1);
            long long rev_low = 0;
            for (int j = 0; j < tt; j++) {
                if ((low_t >> j) & 1) rev_low |= (1LL << (tt - 1 - j));
            }
            int start_m = (tt == 0 ? 1 : 0);
            for (int m = start_m; m <= 30; m++) {
                int hii = m + 2 * tt;
                if (hii > 60)
                    continue;
                if ((n >> hii) != 0)
                    continue;
                long long seg = (n >> tt) & ((1LL << m) - 1);
                if (!check(seg, m)) continue;
                long long high_t = (n >> (m + tt)) & ((1LL << tt) - 1);
                if (high_t == rev_low) {
                    flg = true;
                    break;
                }
            }
        }
        if (flg) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}