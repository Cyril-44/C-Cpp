#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        string a, b;
        cin >> a >> b;

        // Prefix sums of ones in a and b
        vector<int> A1(n+1, 0), B1(n+1, 0);
        for (int i = 1; i <= n; i++) {
            A1[i] = A1[i-1] + (a[i-1] == '1');
            B1[i] = B1[i-1] + (b[i-1] == '1');
        }

        // Build column‐info array: for each y, U[y] = 2*B1[y] - y,
        // and B0[y] = y - B1[y].
        struct Col { 
            int U; 
            int b1; 
            int b0; 
        };
        vector<Col> cols(n);
        for (int y = 1; y <= n; y++) {
            int ones = B1[y];
            cols[y-1] = {
                /* U  = */ 2*ones - y,
                /* b1 = */ ones,
                /* b0 = */ y - ones
            };
        }

        // Sort columns by U ascending
        sort(cols.begin(), cols.end(),
             [](auto &L, auto &R) { return L.U < R.U; });

        // Build prefix sums over the sorted columns
        // prefB1[i] = sum of cols[0..i-1].b1
        // prefB0[i] = sum of cols[0..i-1].b0
        vector<ll> prefB1(n+1, 0), prefB0(n+1, 0);
        for (int i = 1; i <= n; i++) {
            prefB1[i] = prefB1[i-1] + cols[i-1].b1;
            prefB0[i] = prefB0[i-1] + cols[i-1].b0;
        }
        ll totalB0 = prefB0[n];

        // Now accumulate the answer
        ll answer = 0;
        for (int x = 1; x <= n; x++) {
            // Number of ones in a[1..x]
            int onesA = A1[x];
            // A0 = number of zeros in a[1..x]
            int zerosA = x - onesA;
            // T[x] = 2*onesA - x, so K = -T[x] = x - 2*onesA
            int K = x - 2*onesA;

            // Find how many columns y have U[y] <= K
            // i.e. upper_bound on cols.U
            int cnt0 = int(
                upper_bound(
                    cols.begin(), cols.end(), K,
                    [](int v, const Col &C) { return v < C.U; })
                - cols.begin()
            );
            int cnt1 = n - cnt0;

            // Sum of f(x, y) over y where cost0 <= cost1:
            // cost0 = A1[x] + B1[y]
            ll sum0 = ll(cnt0) * onesA + prefB1[cnt0];

            // Sum of f(x, y) over y where cost0 > cost1:
            // cost1 = A0[x] + B0[y]
            ll sum1 = ll(cnt1) * zerosA + (totalB0 - prefB0[cnt0]);

            answer += sum0 + sum1;
        }

        cout << answer << "\n";
    }

    return 0;
}
