#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;
    vector<long long> A(N);
    for (auto &x : A) cin >> x;
    sort(A.begin(), A.end());

    if (N < K) {
        cout << -1 << '\n';
        return 0;
    }

    const long long INF = (1LL << 62);
    vector<long long> dp(N + 1, INF);
    dp[0] = 0;
    long long best = INF;

    for (int i = K; i <= N; ++i) {
        int j = i - K;
        if (dp[j] < INF) best = min(best, dp[j] - A[j]);
        dp[i] = A[i - 1] + best;
    }

    cout << dp[N] << '\n';
    return 0;
}
