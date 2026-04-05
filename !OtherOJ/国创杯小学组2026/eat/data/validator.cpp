#include "testlib.h"
#include <bits/stdc++.h>
#define Checker(N, M, Agent1, Agent2, Agent3)                                                                          \
    []() {                                                                                                             \
        int n = inf.readInt(1, (int)N, "N"); inf.readSpace();                                                          \
        int m = inf.readInt(1, (int)M, "M"); inf.readEoln();                                                           \
        for (int i = 1, ai = -1, now; i <= n; i++) {                                                                   \
            now = inf.readInt(1, (int)1e9, "A_i");                                                                     \
            Agent1;                                                                                                    \
            if (i == n) inf.readEoln();                                                                                \
            else inf.readSpace();                                                                                      \
        }                                                                                                              \
        Agent2;                                                                                                        \
        for (int j = 1; j <= m; j++) {                                                                                 \
            int p = inf.readInt(0, (int)1e6, "p"); inf.readSpace();                                                    \
            long long x = inf.readLong(1ll, (long long)1e18, "x"); inf.readEoln();                                     \
            Agent3;                                                                                                    \
        }                                                                                                              \
        inf.readEof();                                                                                                 \
    }
#define CheckA if (i > 1) ensuref(now == ai, "Spec. A failed"); else ai = now
#define CheckB for (int j = 1; j <= m; j++) { ans.readChar(); ans.readSpace(); ans.readInt(1, 10, "In Spec. B eaten pieces"); ans.readEoln(); }
#define CheckC ensuref(p == 0, "Spec. B failed")
void (*val[])() {
    Checker(10, 10, , , ),
    Checker(1e3, 1e3, , , ),
    Checker(1e5, 1e3, , , ),
    Checker(1e6, 1e6, CheckA, , ),
    Checker(1e6, 1e6, , CheckB, ),
    Checker(1e6, 1e6, , , CheckC),
    Checker(1e6, 1e6, , , )
};
constexpr int MapToSubtask[]{1,2,3,3,4,5,5,5,6,6,6,6,7,7,7,7,7,7,7,7,7};
int main(int argc, char **argv) {
    registerValidation(argc, argv);
    prepareOpts(argc, argv);
    ans.init(opt<std::string>("ans"), _answer);
    int C = inf.readInt(1, 20, "TestID"); inf.readEoln();
    val[MapToSubtask[C-1]-1]();
}
