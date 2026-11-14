#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // Disjoint intervals of occupied integer coordinates: left endpoint -> right endpoint.
    map<long long, long long> intervals;
    long long piece = 0;

    for (int i = 0; i < N; ++i) {
        long long x;
        cin >> x;

        auto it = intervals.upper_bound(x);
        bool covered = false;
        if (it != intervals.begin()) {
            auto prev = std::prev(it);
            if (prev->second >= x) covered = true;
        }

        if (!covered) {
            long long left = x, right = x;
            it = intervals.lower_bound(x);

            if (it != intervals.begin()) {
                auto prev = std::prev(it);
                if (prev->second + 1 == x) {
                    left = prev->first;
                    intervals.erase(prev);
                }
            }

            it = intervals.lower_bound(x);
            if (it != intervals.end() && it->first == x + 1) {
                right = it->second;
                intervals.erase(it);
            }
            intervals[left] = right;
        }

        if (piece == x) {
            auto containing = intervals.upper_bound(x);
            --containing;
            long long left = containing->first;
            long long right = containing->second;
            long long left_distance = x - left + 1;
            long long right_distance = right - x + 1;
            piece = (right_distance <= left_distance) ? right + 1 : left - 1;
        }
    }

    cout << piece << '\n';
    return 0;
}
