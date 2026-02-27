#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int T, n, m, op, x, y, s;
int st[100010], ed[100010], p[100010], id[100010];
long long Ans, real_Ans;

int main(int argc, char **argv) {

    registerTestlibCmd(argc, argv);
    // registerLemonChecker(argc, argv);
    // int Full_Score = atoi(argv[4]);

    T = inf.readInt();
    for (int ts = 1; ts <= T; ts++) {
        n = inf.readInt();
        m = inf.readInt();

        for (int i = 1; i <= n; ++i) {
            st[i] = inf.readInt();
            ed[i] = inf.readInt();
            p[i] = st[i], id[i] = i;
        }

        Ans = ouf.readLong();
        real_Ans = ans.readLong();
        if (Ans != real_Ans) quitf(_wa, "Wrong Answer on testcase %d: Minimum Chaos Value mismatch.", ts);

        s = ouf.readInt(0, 400000, "Operation Counter on testcase " + to_string(ts));

        for (int i = 1; i <= s; ++i) {
            op = ouf.readInt(0, 1, "Operation " + to_string(i) + "'s type on testcase " + to_string(ts));
            if (op == 0) {
				x = ouf.readInt(1, n, "(Type 0) op[" + to_string(i) + "].x, testcase " + to_string(ts));
				y = ouf.readInt(1, m, "(Type 0) op[" + to_string(i) + "].y, testcase " + to_string(ts));
				if (p[x] == y) quitf(_wa, "Invalid output on testcase %d: Unexpected position conflicts on Operation %d (Type 0, x=%d, pos[x]=%d-->y=%d)", ts, i, x, p[x], y);
                if ((p[x] < y) != (st[x] < ed[x])) quitf(_wa, "Invalid output on testcase %d: Direction mismatch on Operation %d (Type 0, x=%d, pos[x]=%d-->y=%d, st[x]=%d-->ed[x]=%d).", ts, i, x, p[x], y, st[x], ed[x]);
                p[x] = y;
            } else {
                x = ouf.readInt(1, n, "(Type 1) op[" + to_string(i) + "].x, testcase " + to_string(ts));
				y = ouf.readInt(1, n, "(Type 1) op[" + to_string(i) + "].y, testcase " + to_string(ts));
				if (x == y) quitf(_wa, "Invalid output on testcase %d: Unexpected equals on Operation %d (Type 1, x=%d, y=%d)", ts, i, x, y);
                if (p[x] != p[y]) quitf(_wa, "Invalid output on testcase %d: Position mismatch on Operation %d (Type 1, x=%d, y=%d, px=%d != py=%d)", ts, i, x, y, p[x], p[y]);
                swap(id[x], id[y]);
            }
        }
        for (int i = 1; i <= n; ++i)
            if (p[i] != ed[i]) quitf(_wa, "Wrong Answer on testcase %d: Expected final position of %d on %d, found %d", ts, i, ed[i], p[i]);

        long long res = 0;
        for (int i = 1; i <= n; ++i) res += abs(ed[i] - st[id[i]]);
        if (res != Ans) quitf(_wa, "Wrong Answer on testcase %d: Your solution's Chaos(%lld) does not match the Min Chaos(%lld).", ts, res, Ans);
    }
    quitf(_ok, "Accepted!");
    return 0;
}
