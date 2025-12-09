#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    freopen("block.in", "r", stdin);
    freopen("block.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        ll a;
        cin >> n >> a;
        vector<pair<ll,ll>> seg; // (L, R)
        seg.reserve(n);
        for (int i = 0; i < n; ++i) {
            ll L, R;
            cin >> L >> R;
            seg.emplace_back(L, R);
        }
        // compute latest start = R - a, check feasibility
        vector<pair<ll,ll>> intervals; // (latest_start, L)
        intervals.reserve(n);
        bool ok = true;
        for (auto &p : seg) {
            ll L = p.first, R = p.second;
            ll latest = R - a;
            if (latest < L) { ok = false; break; }
            intervals.emplace_back(latest, L);
        }
        if (!ok) {
            cout << "no\n";
            continue;
        }
        sort(intervals.begin(), intervals.end()); // sort by latest (then by L)
        ll cur = LLONG_MIN / 4; // sufficiently small
        // We can start from very small; but since L >= 0 in constraints, using 0 is fine.
        // Use 0 to be safe with constraints:
        cur = 0;
        for (auto &it : intervals) {
            ll latest = it.first;
            ll L = it.second;
            ll start = max(cur, L);
            if (start > latest) { ok = false; break; }
            cur = start + a;
        }
        cout << (ok ? "yes" : "no") << '\n';
    }
    return 0;
}
