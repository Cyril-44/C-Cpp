#include "testlib.h"
using namespace std;
inline void check(void) {
    int n = inf.readInt(1, 1000000);
    vector<int> y(n), b(n), vis(n, 0);
    for (auto &x : y) x = inf.readInt(1, n);
    for (auto &x : b) x = inf.readInt(1, n);
    int pans = ouf.readInt(), jans = ans.readInt();
    if (pans == -1 && jans != -1) {
        quitf(_wa, "There is a valid construction but you didn't find it.");
    }
    if (pans == -1) return;
    if (pans < 0 || pans > n) {
        quitf(_wa, "`s` must be within the range of [-1, n].");
    }
    for (int i = 0; i < pans; i++) {
        int p = ouf.readInt();
        if (p <= 0 || p > n || vis[p - 1]) {
            quitf(_wa, "Invalid swap operation.");
        }
        vis[--p] = 1, swap(y[p], b[p]);
    }
    sort(y.begin(), y.end());
    sort(b.begin(), b.end());
    if (y != b) {
        quitf(_wa, "`y` is not a rearrangement of `b` after all the operations.");
    }
    if (jans == -1) {
        quitf(_fail, "You found a valid construction but the jury didn't.");
    }
}
int main(int argc, char *argv[]) {
    registerTestlibCmd(4, argv);
    int T = inf.readInt(1, 10000);
    for (int t=1; t<=T; t++) check();
    quitf(_ok, "Accepted.");
    return 0;
}
