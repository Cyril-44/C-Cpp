#include <bits/stdc++.h>
using namespace std;

bool check(long long n, int l) {
    if (l < 1) return false;
    if (n >= (1LL << l)) return false;
    int half = (l - 1) / 2;
    for (int j = 0; j <= half; j++) {
        int bit_j = (n >> j) & 1;
        int paired = l - 1 - j;
        int bit_paired = (paired >= 30) ? 0 : ((n >> paired) & 1);
        if (bit_j != bit_paired) return false;
    }
    if (l % 2 == 1) {
        int mid = (l - 1) / 2;
        int bit_mid = (mid >= 30) ? 0 : ((n >> mid) & 1);
        if (bit_mid != 0) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    for (int test = 0; test < t; test++) {
        long long n;
        cin >> n;
        bool exists = false;
        for (int l = 1; l <= 60; l++) {
            if (check(n, l)) {
                exists = true;
                break;
            }
        }
        if (exists) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}