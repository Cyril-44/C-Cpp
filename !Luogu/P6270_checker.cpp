#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    int T = inf.readInt(1, 1000, "T");

    for (int tc = 1; tc <= T; ++tc) {
        int n = inf.readInt(1, 100000, "n");
        int m = inf.readInt(1, 1000000, "m");

        vector<int> s(n + 1), e(n + 1);
        for (int i = 1; i <= n; ++i) {
            s[i] = inf.readInt(1, m, "s_i");
            e[i] = inf.readInt(1, m, "e_i");
            if (s[i] == e[i]) {
                quitf(_fail, "Input error: s[%d] == e[%d] in test case %d", i, i, tc);
            }
        }

        // Jury's answer (minimal cost)
        long long juryAns = ans.readLong(0, (long long)1e18, "juryAns");
        long long juryS   = ans.readLong(0, (long long)400000, "juryS");

        // Skip jury operations (we only trust juryAns as minimal)
        for (long long i = 0; i < juryS; ++i) {
            int type = ans.readInt(0, 1, "juryType");
            int x    = ans.readInt(1, n, "juryX");
            int y    = ans.readInt(1, (type == 0 ? m : n), "juryY");
            (void)type; (void)x; (void)y;
        }

        // Contestant's answer
        if (ouf.eof()) {
            quitf(_wa, "Unexpected EOF in contestant output before test case %d header", tc);
        }

        long long contAns = ouf.readLong(0, (long long)1e18, "contAns");
        long long contS   = ouf.readLong(0, (long long)400000, "contS");

        // Prepare simulation state
        vector<int> curPos(n + 1), cardStart(n + 1);
        for (int i = 1; i <= n; ++i) {
            curPos[i]    = s[i];
            cardStart[i] = s[i];
        }

        // Simulate contestant operations
        for (long long opId = 1; opId <= contS; ++opId) {
            if (ouf.eof()) {
                quitf(_wa,
                      "Unexpected EOF in contestant output while reading operation %lld of test case %d "
                      "(expected %lld operations)",
                      opId, tc, contS);
            }

            int type = ouf.readInt(0, 1, "type");
            int x    = ouf.readInt(1, n, "x");

            if (type == 0) {
                int y = ouf.readInt(1, m, "y");

                if (y == curPos[x]) {
                    quitf(_wa,
                          "Invalid move in test case %d, operation %lld: person %d moves from station %d to same station %d",
                          tc, opId, x, curPos[x], y);
                }

                int ex = e[x];
                int cx = curPos[x];

                if (ex > cx) {
                    // Must move strictly forward, not beyond e[x]
                    if (!(y > cx && y <= ex)) {
                        quitf(_wa,
                              "Invalid forward move in test case %d, operation %lld: person %d at %d with target %d "
                              "cannot move to %d (must satisfy %d < y <= %d)",
                              tc, opId, x, cx, ex, y, cx, ex);
                    }
                } else {
                    // ex < cx: must move strictly backward, not beyond e[x]
                    if (!(y < cx && y >= ex)) {
                        quitf(_wa,
                              "Invalid backward move in test case %d, operation %lld: person %d at %d with target %d "
                              "cannot move to %d (must satisfy %d > y >= %d)",
                              tc, opId, x, cx, ex, y, cx, ex);
                    }
                }

                curPos[x] = y;
            } else { // type == 1
                int y = ouf.readInt(1, n, "y");

                if (curPos[x] != curPos[y]) {
                    quitf(_wa,
                          "Invalid swap in test case %d, operation %lld: persons %d and %d are at different stations "
                          "(%d vs %d)",
                          tc, opId, x, y, curPos[x], curPos[y]);
                }

                std::swap(cardStart[x], cardStart[y]);
            }
        }

        // After all operations, everyone must be at their destination
        for (int i = 1; i <= n; ++i) {
            if (curPos[i] != e[i]) {
                quitf(_wa,
                      "Final position mismatch in test case %d: person %d ends at station %d, expected %d",
                      tc, i, curPos[i], e[i]);
            }
        }

        // Compute actual total cost
        long long realCost = 0;
        for (int i = 1; i <= n; ++i) {
            long long c = llabs((long long)cardStart[i] - (long long)e[i]);
            realCost += c;
        }

        // Check declared answer vs simulated cost
        if (contAns != realCost) {
            quitf(_wa,
                  "Declared total cost mismatch in test case %d: contestant printed %lld, but simulated cost is %lld",
                  tc, contAns, realCost);
        }

        // Check optimality vs jury answer
        if (realCost != juryAns) {
            if (realCost < juryAns) {
                quitf(_fail,
                      "Contestant achieves cost %lld < jury minimal %lld in test case %d (jury solution incorrect)",
                      realCost, juryAns, tc);
            } else {
                quitf(_wa,
                      "Suboptimal solution in test case %d: total cost %lld > minimal %lld",
                      tc, realCost, juryAns);
            }
        }
    }

    // Ensure no extra tokens

    quitf(_ok, "All test cases passed: contestant solution is valid and optimal");
}
