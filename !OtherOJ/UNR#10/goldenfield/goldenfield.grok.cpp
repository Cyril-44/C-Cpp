#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll compute_final_sum(const vector<ll>& b, int l, int r) {  // 0-based l to r inclusive? wait 1-based adjust
    int m = r - l + 1;
    if (m <= 2) {
        ll s = 0;
        for(int i=l; i<=r; i++) s += b[i];
        return s;
    }
    vector<ll> a(m);
    for(int i=0; i<m; i++) a[i] = b[l + i];
    deque<int> q;
    for(int i=1; i<m-1; i++) q.push_back(i);
    int iterations = 0;
    while (!q.empty() && iterations < m*2) {
        iterations++;
        int i = q.front(); q.pop_front();
        if (i < 1 || i >= m-1) continue;
        if (a[i] < a[i-1] && a[i] < a[i+1]) {
            ll new_val = max(a[i-1], a[i+1]);
            if (new_val > a[i]) {
                a[i] = new_val;
                if (i-1 >= 1) q.push_back(i-1);
                if (i+1 < m-1) q.push_back(i+1);
            }
        }
    }
    ll sum = 0;
    for(ll x : a) sum += x;
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> b(n);
    for(int i=0; i<n; i++) cin >> b[i];
    for(int iq=0; iq<q; iq++) {
        int l, r;
        cin >> l >> r;
        l--; r--;  // to 0-based
        cout << compute_final_sum(b, l, r) << '\n';
    }
    return 0;
}