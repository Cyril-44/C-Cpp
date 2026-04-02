#include <bits/stdc++.h>
using namespace std;

bool is_pal(const string &s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r]) return false;
        ++l; --r;
    }
    return true;
}

bool is_nondec(const string &s, int l, int r) {
    for (int i = l; i < r; ++i) {
        if (s[i] > s[i + 1]) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        string s;
        cin >> s;
        int n = s.size();
        int ans = 0;
        int i = 0;
        while (i < n) {
            int best = i;
            for (int j = i; j < n; ++j) {
                if (is_pal(s, i, j) && is_nondec(s, i, j)) {
                    best = j;
                }
            }
            ++ans;
            i = best + 1;
        }
        cout << ans << "\n";
    }
    return 0;
}
