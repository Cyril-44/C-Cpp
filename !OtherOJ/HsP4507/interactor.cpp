#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

long long score_sub1(long long Q) {
    if (Q <= 50) return 13;
    if (Q <= 200) return 9;
    if (Q <= 5040) return 6;
    return 0;
}

double score_sub2(long long Q) {
    long long Qp = (Q / 100 + 1) * 100;
    if (Qp <= 400) return 38;
    if (Qp <= 700) return (38 - 29) * (700 - Qp) / 300.0 + 29;
    if (Qp <= 1300) return (29 - 21) * (1300 - Qp) / 600.0 + 21;
    if (Qp <= 10000) return (21 - 4) * (10000 - Qp) / 8700.0 + 4;
    return 0;
}

double score_sub3(long long Q) {
    long long Qp = (Q / 100 + 1) * 100;
    if (Qp <= 2400) return 49;
    if (Qp <= 5000) return (49 - 29) * (5000 - Qp) / 2600.0 + 29;
    return 0;
}

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    int sub = inf.readInt(1, 3, "Sub"), T = inf.readInt();

    long long worstQ = 0;
    cout << T << endl; cout.flush();
    for (int _ = 0; _ < T; _++) {
        int n = inf.readInt();
        int mode = inf.readInt();
        uint64_t seed = inf.readUnsignedLong();

        cout << n << " " << mode << " " << seed << "\n";
        cout.flush();
        long long Q = ouf.readLong(-(1ll << 30), 1ll << 40, "Q");
        if (Q == 1ll << 32) quitf(_wa, "Failed, tries to query with an incompatible array (length mismatch).");
        if (Q == 1ll << 33) quitf(_wa, "Failed, Queried too much (> 10000 times).");
        if (Q == 1ll << 34) quitf(_wa, "Failed, tries to query with an incompatible array (value not valid).");
        if (Q == 1ll << 35) quitf(_wa, "Failed, tries to query with an incompatible array (not a permutation).");
        if (Q <= 0) quitf(_wa, "Wrong answer, the permutation was not guessed.");
        if (Q > worstQ) {
            worstQ = Q;
        }
    }

    double score = 0;
    if (sub == 1) score = score_sub1(worstQ);
    if (sub == 2) score = score_sub2(worstQ);
    if (sub == 3) score = score_sub3(worstQ);

    double full = (sub == 1 ? 13 : sub == 2 ? 38 : 49);

    if (score < full)
        quitp(score);

    quitf(_ok, "Max query = %lld", worstQ);
}
