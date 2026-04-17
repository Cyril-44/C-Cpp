#include "testlib.h"
#include <vector>
#include <set>

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 1000000, "n");
    inf.readSpace();
    int q = inf.readInt(1, 1000000, "q");
    inf.readChar('\n');

    std::vector<int> a = inf.readInts(n, 1, n, "a");
    inf.readChar('\n');
    
    std::set<int> s(a.begin(), a.end());
    ensuref(s.size() == n, "Not a permutation");

    for (int i = 0; i < q; ++i) {
        int l = inf.readInt(1, n, "l");
        inf.readSpace();
        int r = inf.readInt(1, n, "r");
        inf.readSpace();
        int k = inf.readInt(1, 1000000, "k");
        inf.readChar('\n');
        ensuref(l <= r, "l must be <= r");
    }
    inf.readEof();
    return 0;
}