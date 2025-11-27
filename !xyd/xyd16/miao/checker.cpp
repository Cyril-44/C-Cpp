#include <bits/stdc++.h>
#include <testlib.h>
#include <gmpxx.h>
constexpr int N = 2000005;
int a[N];
int main(int argc, char *argv[]) {
    mpq_class mql;
    registerTestlibCmd(argc, argv);
    int T = inf.readInt();
    for (int t_i = 1; t_i <= T; t_i++) {
        int n = inf.readInt();
        for (int i = 1; i <= n; i++)
            a[i] = inf.readInt();
        std::string token = ouf.readToken("Yes|No", "judge");
        if (token == "Yes") {
            int opt = ouf.readInt(0, 2*n, "op count");
            std::stack<int> sta;
            int ptr = 0;
            for (int op_i = 1; op_i <= opt; ++op_i) {
                int op = ouf.readInt(1, 2, "op");
                if (op == 1) {
                    if (ptr == n) quitf(_wa, "On operation %d, testcase %d: Trying to fetch from an empty card array!", op_i, t_i);
                    sta.push(a[++ptr]);
                } else {
                    int x = ouf.readInt(2, n, "x");
                    if (x > (int)sta.size()) quitf(_wa, "On operation %d, testcase %d: There's only %zu cards in the stack, but you wanted to pop %d!", op_i, t_i, sta.size(), x);
                    int u = sta.top();
                    while (x--) {
                        if (sta.top() != u) quitf(_wa, "On operation %d, testcase %d, the top %d cards in the stack is not the same!", op_i, t_i, x);
                        sta.pop();
                    }
                }
            }
            if (!sta.empty()) quitf(_wa, "On testcase %d, the stack wasn't empty eventually with %zu cards left!", t_i, sta.size());
            if (ptr != n) quitf(_wa, "On testcase %d, you left %d cards!", t_i, n - ptr);
        }
    }
    quitf(_ok, "Accepted!");
    return 0;
}
