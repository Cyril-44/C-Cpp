#include <bits/stdc++.h>
#include "testlib.h"
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else 
# define debug(...) ((void)0)
#endif
using namespace std;
inline int getx(vector<int> a) {
    int n = a.size();
    sort(a.rbegin(), a.rend());
    vector<int> layer(n), hlayers(n + 1);
    vector<bool> flag(n + 1);
    vector<int> hflags(n + 1);
    ++a.front();
    for (int l = 0, r = 1; l < r; ++l)
        for (int _ = a[l]; --_; ++r)
            layer[r] = layer[l] + 1, flag[r] = flag[l] || r == 1;
    for (int i : layer) ++hlayers[i];
    for (int i = 1; i <= n; i++) if (flag[i]) ++hflags[layer[i]];
    int mx1 = 0, mx2 = 0;
    for (int i = 1; i <= n; i++) {
        if (hflags[i]) mx1 = i;
        if (hlayers[i] - hflags[i]) mx2 = i;
    }
    return mx1 + mx2;
}
inline int gety(const vector<int>& a) {
    int cnt = 0;
    for (int i : a) if (i == 1) ++cnt;
    return (int)a.size() - cnt + 1;
}
int main(int argc, char** argv) {
    registerTestlibCmd(argc, argv);
    int n = inf.readInt(), x = inf.readInt(), y = inf.readInt();
    string answer = ans.readToken();
    if (answer == "-1") {
        string d = ouf.readToken("-1");
        quitf(_ok, "Accepted, no answer.");
    }
    if (x == -1) x = n - 1;
    if (y == -1) y = 2;
    vector<int> a(n);
    int sum = 0;
    for (int &i : a) {
        i = ouf.readInt();
        if (i < 1 || i >= n) quitf(_wa, "Cannot form a tree.");
        sum += i;
    }
    if (sum != 2 * n - 2) quitf(_wa, "Sequence sum unexpected %d", sum);
    debug("%d %d\n", getx(a), gety(a));
    if (getx(a) != x || gety(a) != y) quitf(_wa, "Your answer does not fit the input.");
    quitf(_ok, "Acceptable sequence.");
    return 0;
}
