#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    constexpr int64 MAXK = 1000000000000000000LL;
    const int task = inf.readInt(1, 7, "T");
    const int64 K = inf.readLong(1LL, MAXK, "K");
    inf.skipBlanks();
    if (!inf.seekEof()) {
        quitf(_fail, "extra content after T and K in the input");
    }

    if ((task == 1 && K > 5) || (task == 2 && K > 10) ||
        (task == 5 && K > 1000) ||
        (task == 6 && __builtin_popcountll(static_cast<unsigned long long>(K)) != 2)) {
        quitf(_fail, "input violates subtask %d property", task);
    }

    const int n = ouf.readInt(1, 10, "N");
    const int m = ouf.readInt(1, 10000, "M");

    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        string row = ouf.readToken();
        if (static_cast<int>(row.size()) != m) {
            quitf(_pe, "row %d: expected length %d, found %d",
                  i + 1, m, static_cast<int>(row.size()));
        }
        for (int j = 0; j < m; ++j) {
            if (row[j] != '0' && row[j] != '1') {
                quitf(_pe, "row %d, column %d: expected '0' or '1'",
                      i + 1, j + 1);
            }
        }
        grid[i] = std::move(row);
    }

    ouf.skipBlanks();
    if (!ouf.seekEof()) {
        quitf(_pe, "extra content after the grid");
    }

    // Saturation at K+1 preserves the exact comparison with K while avoiding overflow.
    const int64 CAP = K + 1;
    auto add = [CAP](int64& x, int64 v) {
        x += v;
        if (x > CAP) x = CAP;
    };

    const int states = 1 << n;
    vector<int64> cur(states, 0), nxt(states, 0);
    cur[0] = 1;

    // Process cells column by column using a frontier mask.
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            fill(nxt.begin(), nxt.end(), 0);
            const bool white = grid[i][j] == '0';
            const bool canH = white && j + 1 < m && grid[i][j + 1] == '0';
            const bool canV = white && i + 1 < n && grid[i + 1][j] == '0';

            for (int mask = 0; mask < states; ++mask) {
                const int64 ways = cur[mask];
                if (ways == 0) continue;

                if ((mask >> i) & 1) {
                    add(nxt[mask ^ (1 << i)], ways);
                } else if (!white) {
                    add(nxt[mask], ways);
                } else {
                    if (canH) add(nxt[mask | (1 << i)], ways);
                    if (canV && !((mask >> (i + 1)) & 1))
                        add(nxt[mask | (1 << (i + 1))], ways);
                }
            }
            cur.swap(nxt);
        }
    }

    const int64 count = cur[0];
    if (count != K) {
        if (count >= CAP)
            quitf(_wa, "tiling count is greater than K=%lld", K);
        quitf(_wa, "tiling count is %lld, expected %lld", count, K);
    }

    if (task <= 5) {
        quitf(_ok, "valid construction; N=%d, M=%d, count=%lld", n, m, K);
    }

    const int totalPoints = task == 6 ? 20 : 50;
    const long double t = logl(max(m, 180) / 180.0L) /
                          logl(10000.0L / 180.0L);
    const long double fPercent = 100.0L - 84.0L * t - 15.0L * sqrtl(t) + 4.0L * t * t;
    const long double g = powl(0.8L, max(n - 4, 0));
    long double rate = (fPercent / 100.0L) * g;
    if (rate < 0.0L) rate = 0.0L;
    if (rate > 1.0L) rate = 1.0L;
    const int earned = static_cast<int>(floorl(totalPoints * rate + 1e-12L));
    const double partial = static_cast<double>(earned) / totalPoints;
    quitp(partial, format("valid construction; N=%d, M=%d, score=%d/%d",
                          n, m, earned, totalPoints));
}
