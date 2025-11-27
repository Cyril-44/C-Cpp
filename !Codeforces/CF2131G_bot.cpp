#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1000000007;

// Precomputed up to m = 31
long long ffsub[32];    // fsub[m] = number of nodes in subtree T(m) = 2^(m-1)
long long P[32];       // P[m] = product of all values in T(m) mod
long long cumP[32];    // cumP[m] = (P[1] * P[2] * ... * P[m]) mod

// Compute prefix product of the first k elements of T(m)
long long prefixProd(int m, long long k) {
    if (k <= 0) return 1LL;
    if (m == 1) return 1LL; // T(1) = [1, 1, 1, ...] so product = 1

    long long ans = 1;
    // take the first element m
    if (k >= 1) {
        ans = (ans * m) % MOD;
        k--;
        if (k == 0) 
            return ans;
    }

    // then consume from concatenated T(1), T(2), ..., T(m-1)
    for (int j = 1; j < m && k > 0; ++j) {
        long long blockSize = ffsub[j];
        if (blockSize <= k) {
            // take full block T(j)
            ans = (ans * P[j]) % MOD;
            k -= blockSize;
        } else {
            // partial of T(j)
            ans = (ans * prefixProd(j, k)) % MOD;
            k = 0;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Precompute fsub and P up to 31
    ffsub[1] = 1;
    for (int i = 2; i <= 31; ++i) {
        ffsub[i] = ffsub[i - 1] << 1; // 2^(i-1)
    }

    cumP[0] = 1;
    for (int m = 1; m <= 31; ++m) {
        // P[m] = m * (P[1] * P[2] * ... * P[m-1])
        P[m] = ( (long long)m * cumP[m - 1] ) % MOD;
        cumP[m] = (cumP[m - 1] * P[m]) % MOD;
    }

    int t;
    cin >> t;
    while (t--) {
        int n;
        long long k;
        cin >> n >> k;

        vector<long long> S(n);
        for (int i = 0; i < n; ++i) {
            cin >> S[i];
        }
        sort(S.begin(), S.end());

        long long rem = k;
        long long answer = 1;

        for (long long s : S) {
            if (rem == 0) break;

            // Compute subtree size tot = 2^(s-1)
            // If s >= 32 then tot > 1e9 >= rem
            bool fullTree = false;
            long long tot = 0;
            if (s <= 31) {
                tot = ffsub[s];
                if (tot <= rem) fullTree = true;
            }

            if (fullTree) {
                // include entire T(s)
                answer = (answer * P[s]) % MOD;
                rem -= tot;
            } else {
                // partial cover inside T(s)
                // first remove root s
                answer = (answer * (s % MOD)) % MOD;
                rem--;
                if (rem == 0) break;

                // consume from T(1)..T(s-1)
                // but rem <= 1e9, so we only need j up to ~31
                for (int j = 1; rem > 0 && j < 32; ++j) {
                    long long blockSize = ffsub[j];
                    if (blockSize <= rem) {
                        answer = (answer * P[j]) % MOD;
                        rem -= blockSize;
                    } else {
                        answer = (answer * prefixProd(j, rem)) % MOD;
                        rem = 0;
                    }
                }
                break;
            }
        }

        cout << answer % MOD << "\n";
    }

    return 0;
}
