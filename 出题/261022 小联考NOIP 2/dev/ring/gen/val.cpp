#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    prepareOpts(argc, argv); // This repository testlib does not prepare custom validator options.
    const int group = has_opt("subtask") ? opt<int>("subtask") : 4;
    ensuref(1 <= group && group <= 4, "subtask must be in [1, 4]");
    const int n = inf.readInt(4, 1'000'000, "n");
    inf.readEoln();
    vector<int> child_count(n + 1);
    bool star = true;
    for (int u = 2; u <= n; ++u) {
        if (u > 2) inf.readSpace();
        int p = inf.readInt(1, u - 1, "p_i");
        ++child_count[p];
        star &= p == 1;
    }
    inf.readEoln();
    inf.readEof();

    int leaves = 0;
    for (int u = 1; u <= n; ++u) leaves += child_count[u] == 0;
    if (group == 1) ensuref(n <= 20, "Subtask 1 requires n <= 20");
    if (group == 2) {
        ensuref(n <= 100'000, "Subtask 2 requires n <= 100000");
        ensuref(leaves <= 8, "Subtask 2 requires at most 8 leaves");
    }
    if (group == 3) ensuref(star, "Subtask 3 requires a star rooted at 1");
}
