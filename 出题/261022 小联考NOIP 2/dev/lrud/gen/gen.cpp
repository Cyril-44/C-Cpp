#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

bool permitted(int n, int st) {
    if (n < 2 || n > 1000) return false;
    if (st == 1) return n <= 3;
    if (st == 2) return n <= 4;
    if (st == 3) return n <= 4 || n % 2 == 1;
    if (st == 4) return n <= 4 || n % 4 != 0;
    return true;
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    int st = opt<int>("subtask", 5);
    string mode = opt<string>("mode", "random");
    int t = opt<int>("t", 5);
    int seed = opt<int>("seed", 1);
    int anchor = opt<int>("anchor", -1);
    string values = opt<string>("values", "");
    ensuref(1 <= st && st <= 5, "subtask must be in [1,5]");
    ensuref(1 <= t && t <= 5, "t must be in [1,5]");
    rnd.setSeed(seed);
    vector<int> allowed, ns;
    for (int n = 2; n <= 1000; ++n) if (permitted(n, st)) allowed.push_back(n);
    if (mode == "list") {
        ensuref(!values.empty(), "list mode requires --values=2,3,...");
        replace(values.begin(), values.end(), ',', ' ');
        istringstream in(values);
        int x;
        while (in >> x) ns.push_back(x);
        ensuref(in.eof(), "invalid list of values");
        ensuref(!ns.empty() && ns.size() <= 5, "list needs 1..5 values");
    } else if (mode == "max") {
        ns.assign(t, allowed.back());
    } else if (mode == "random" || mode == "large") {
        int lo = mode == "large" ? max(0, (int)allowed.size() - 30) : 0;
        for (int i = 0; i < t; ++i) ns.push_back(allowed[rnd.next(lo, (int)allowed.size() - 1)]);
    } else {
        quitf(_fail, "unknown mode %s", mode.c_str());
    }
    if (anchor != -1) ns[0] = anchor;
    for (int n : ns) ensuref(permitted(n, st), "N=%d violates subtask %d", n, st);
    cout << ns.size() << '\n';
    for (int n : ns) cout << n << '\n';
}
