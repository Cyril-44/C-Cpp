#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, P;
    cin >> N >> P;
    vector<long long> T(N);
    for (auto &x : T) cin >> x;

    // For a time d, count all watering jobs with deadlines at most d.
    // A plant with initial level T_i has deadlines T_i + jP (j >= 0).
    // The prefix is feasible iff this count is at most d for every d.
    vector<pair<long long, int>> plants;
    plants.reserve(N);
    for (long long x : T) plants.emplace_back(x, (int)(x % P));
    sort(plants.begin(), plants.end());

    auto feasible = [&](long long k) {
        if (k <= 0) return true;
        vector<long long> queries;
        int first_count = (int)min<long long>(P, k);
        queries.reserve(first_count * 2);
        for (int r = 1; r <= first_count; ++r) {
            queries.push_back(r);
            long long last = r + ((k - r) / P) * P;
            queries.push_back(last);
        }
        sort(queries.begin(), queries.end());
        queries.erase(unique(queries.begin(), queries.end()), queries.end());

        vector<int> bit(P + 1, 0);
        auto add = [&](int idx) {
            for (++idx; idx <= P; idx += idx & -idx) ++bit[idx];
        };
        auto sum_prefix = [&](int count) {
            int result = 0;
            for (int i = count; i > 0; i -= i & -i) result += bit[i];
            return result;
        };

        int at = 0;
        long long sum_quotients = 0;
        for (long long d : queries) {
            while (at < N && plants[at].first <= d) {
                sum_quotients += plants[at].first / P;
                add(plants[at].second);
                ++at;
            }
            int r = (int)(d % P);
            int active = at;
            int residues_le_r = sum_prefix(r + 1);
            int residues_gt_r = active - residues_le_r;
            long long q = d / P;
            long long jobs = (long long)active * (q + 1) - sum_quotients - residues_gt_r;
            if (jobs > d) return false;
        }
        return true;
    };

    if (N <= P && feasible(P)) {
        cout << -1 << '\n';
        return 0;
    }

    long long lo = 0, hi;
    if (N <= P) {
        hi = P;
    } else {
        hi = 1;
        while (feasible(hi)) {
            lo = hi;
            hi *= 2;
        }
    }
    while (lo + 1 < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(mid)) lo = mid;
        else hi = mid;
    }
    cout << lo << '\n';
    return 0;
}
