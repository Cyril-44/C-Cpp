#include <bits/stdc++.h>
using namespace std;

static constexpr long long MOD = 1'000'000'007LL;

struct Aggregate {
    long long count = 0;
    long long sumLength = 0;
    long long sumLastNonzero = 0;
    long long sumBoundaryNonzero = 0;
    long long sumAnswer = 0;
};

static inline void addMod(long long &to, long long value) {
    to += value;
    if (to >= MOD) to -= MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string N;
    int D;
    cin >> N >> D;

    // tight=1 means the processed prefix is exactly N's prefix; tight=0
    // means it is already smaller. Unstarted prefixes consist only of zeros.
    long long unstarted[2] = {0, 1};
    Aggregate dp[2][10]{};

    for (int i = 0; i < static_cast<int>(N.size()); ++i) {
        Aggregate next[2][10]{};
        long long nextUnstarted[2] = {0, 0};
        const int boundDigit = N[i] - '0';

        for (int tight = 0; tight <= 1; ++tight) {
            const int limit = tight ? boundDigit : 9;

            for (int digit = 0; digit <= limit; ++digit) {
                const int nextTight = tight && digit == boundDigit;

                if (unstarted[tight] != 0) {
                    const long long ways = unstarted[tight];
                    if (digit == 0) {
                        addMod(nextUnstarted[nextTight], ways);
                    } else {
                        Aggregate &to = next[nextTight][digit];
                        addMod(to.count, ways);
                        addMod(to.sumLength, ways);          // New length is 1.
                        addMod(to.sumLastNonzero, ways);     // Its last nonzero is at 1.
                        addMod(to.sumAnswer, ways);          // The one-digit substring.
                    }
                }

                for (int last = 0; last <= 9; ++last) {
                    const Aggregate &from = dp[tight][last];
                    if (from.count == 0 && from.sumLength == 0 &&
                        from.sumLastNonzero == 0 && from.sumBoundaryNonzero == 0 &&
                        from.sumAnswer == 0) {
                        continue;
                    }

                    const bool smoothEdge = abs(last - digit) <= D;
                    Aggregate &to = next[nextTight][digit];
                    addMod(to.count, from.count);

                    // Every represented prefix grows by one digit.
                    addMod(to.sumLength, from.sumLength);
                    addMod(to.sumLength, from.count);

                    // The latest nonzero position either stays put or becomes
                    // the new position (whose index is the new length).
                    if (digit == 0) {
                        addMod(to.sumLastNonzero, from.sumLastNonzero);
                    } else {
                        addMod(to.sumLastNonzero, from.sumLength);
                        addMod(to.sumLastNonzero, from.count);
                    }

                    // On a broken edge, the smooth suffix starts here, so its
                    // preceding nonzero boundary is the old latest nonzero.
                    addMod(to.sumBoundaryNonzero,
                           smoothEdge ? from.sumBoundaryNonzero : from.sumLastNonzero);

                    // Number of valid substrings ending here is length minus
                    // the last nonzero before the current smooth suffix.
                    addMod(to.sumAnswer, from.sumAnswer);
                    addMod(to.sumAnswer, from.sumLength);
                    addMod(to.sumAnswer, from.count);
                    const long long boundarySum = smoothEdge
                        ? from.sumBoundaryNonzero
                        : from.sumLastNonzero;
                    to.sumAnswer -= boundarySum;
                    if (to.sumAnswer < 0) to.sumAnswer += MOD;
                }
            }
        }

        copy(begin(nextUnstarted), end(nextUnstarted), begin(unstarted));
        memcpy(dp, next, sizeof(dp));
    }

    long long result = 0;
    for (int tight = 0; tight <= 1; ++tight) {
        for (int last = 0; last <= 9; ++last) {
            addMod(result, dp[tight][last].sumAnswer);
        }
    }
    cout << result << '\n';
    return 0;
}
