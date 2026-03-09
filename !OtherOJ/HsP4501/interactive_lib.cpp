// interactor.cpp
#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;
using u64 = unsigned long long;
int main(int argc, char *argv[]) {
    registerInteraction(argc, argv);
    int N = inf.readInt(1, 100000000, "N");
    u64 seed = inf.readUnsignedLong();
    unsigned MOD = inf.readUnsignedLong(0, 0xffffffffull, "MOD");
    cout << N << ' ' << seed << ' ' << MOD << endl;
    cout.flush();
    bool okFlag = ouf.readInt(0, 1, "okFlag");
    if (okFlag) quitf(_ok, "All Mods match (N=%d)", N);
    else quitf(_wa, "Wrong answer on position %d", ouf.readInt(0, N-1, "Wrong Position"));
    return 0;
}
