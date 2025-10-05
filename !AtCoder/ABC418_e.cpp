#include <bits/stdc++.h>
using namespace std;

static inline long long llgcd(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<long long> X(N), Y(N);
    for (int i = 0; i < N; ++i) cin >> X[i] >> Y[i];

    const long long M = 1LL * N * (N - 1) / 2;

    vector<uint64_t> slopes;
    slopes.reserve(M);
    vector<pair<long long,long long>> mids;
    mids.reserve(M);

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            long long dx = X[j] - X[i];
            long long dy = Y[j] - Y[i];
            long long g = llgcd(dx, dy);
            dx /= g; dy /= g;
            if (dy < 0 || (dy == 0 && dx < 0)) { dx = -dx; dy = -dy; }
            uint32_t dx32 = static_cast<uint32_t>(static_cast<int32_t>(dx));
            uint32_t dy32 = static_cast<uint32_t>(static_cast<int32_t>(dy));
            uint64_t key = (uint64_t(dx32) << 32) | uint64_t(dy32);
            slopes.push_back(key);

            mids.emplace_back(X[i] + X[j], Y[i] + Y[j]); // doubled midpoint
        }
    }

    auto count_pairs_from_runs = [](auto &v) -> long long {
        sort(v.begin(), v.end());
        long long res = 0;
        for (size_t i = 0; i < v.size(); ) {
            size_t j = i + 1;
            while (j < v.size() && v[j] == v[i]) ++j;
            long long k = (long long)(j - i);
            res += k * (k - 1) / 2;
            i = j;
        }
        return res;
    };

    long long S = count_pairs_from_runs(slopes); // sum C(M_d, 2)
    long long P = count_pairs_from_runs(mids);   // sum C(k_mid, 2)

    cout << (S - P) << '\n';
    return 0;
}