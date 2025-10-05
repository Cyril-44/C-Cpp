#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n, m, q;
        cin >> n >> m >> q;
        vector<ll> A(n), B(m);
        for (int i = 0; i < n; ++i) cin >> A[i];
        for (int i = 0; i < m; ++i) cin >> B[i];
        sort(A.begin(), A.end(), greater<ll>());
        sort(B.begin(), B.end(), greater<ll>());
        vector<ll> prefA(n+1, 0), prefB(m+1, 0);
        for (int i = 0; i < n; ++i) prefA[i+1] = prefA[i] + A[i];
        for (int i = 0; i < m; ++i) prefB[i+1] = prefB[i] + B[i];

        int Zmax = n + m;
        vector<int> bestK(Zmax + 1, 0);
        vector<ll> bestVal(Zmax + 1, 0);

        int k = 0;
        for (int z = 0; z <= Zmax; ++z) {
            int klow = max(0, z - m);
            int khigh = min(n, z);
            if (k < klow) k = klow;
            if (k > khigh) k = khigh; // ensure within current feasible range
            // try to increase k while it improves the value
            while (k < khigh) {
                // compare value at k and k+1
                ll valNow = prefA[k] + prefB[z - k];
                ll valNext = prefA[k+1] + prefB[z - (k+1)];
                if (valNext >= valNow) ++k;
                else break;
            }
            bestK[z] = k;
            bestVal[z] = prefA[k] + prefB[z - k];
        }

        // answer queries
        while (q--) {
            int x, y, z;
            cin >> x >> y >> z;
            int L = max(0, z - y);
            int R = min(x, z);
            // clamp precomputed bestK[z] to [L, R]
            int kk = bestK[z];
            if (kk < L) kk = L;
            else if (kk > R) kk = R;
            ll ans = prefA[kk] + prefB[z - kk];
            cout << ans << '\n';
        }
    }
    return 0;
}
