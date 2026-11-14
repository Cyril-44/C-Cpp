#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    for (int &x : A) cin >> x;
    vector<int> B(N - 1);
    vector<long long> C(N - 1);
    for (int i = 0; i < N - 1; ++i) cin >> B[i] >> C[i];

    const long long NEG = -(1LL << 60);
    vector<long long> dp(M + 1, NEG), next(M + 1, NEG);
    dp[M - A[0]] = 0;

    for (int i = 0; i < N - 1; ++i) {
        fill(next.begin(), next.end(), NEG);
        for (int h = 0; h <= M; ++h) {
            if (dp[h] == NEG) continue;
            int recovered = min(M, h + B[i]);
            next[recovered] = max(next[recovered], dp[h]);
            next[h] = max(next[h], dp[h] + C[i]);
        }

        fill(dp.begin(), dp.end(), NEG);
        for (int h = A[i + 1]; h <= M; ++h) {
            if (next[h] != NEG) {
                dp[h - A[i + 1]] = max(dp[h - A[i + 1]], next[h]);
            }
        }
    }

    cout << *max_element(dp.begin(), dp.end()) << '\n';
    return 0;
}
