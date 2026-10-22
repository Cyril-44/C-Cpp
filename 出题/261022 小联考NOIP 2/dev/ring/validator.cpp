#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(4, 1'000'000, "n");
    inf.readEoln();

    vector<int> child_count(n + 1, 0);
    vector<int> parent(n + 1, 0);
    for (int u = 2; u <= n; ++u) {
        if (u > 2) inf.readSpace();
        parent[u] = inf.readInt(1, u - 1, "p_i");
        ++child_count[parent[u]];
    }
    inf.readEoln();
    inf.readEof();

    int leaves = 0;
    for (int u = 1; u <= n; ++u) leaves += (child_count[u] == 0);

    if (has_opt("subtask")) {
        int subtask = opt<int>("subtask");
        if (subtask == 1) ensuref(n <= 20, "Subtask 1 requires n <= 20");
        else if (subtask == 2) {
            ensuref(n <= 100'000, "Subtask 2 requires n <= 100000");
            ensuref(leaves <= 8, "Subtask 2 requires at most 8 leaves");
        } else if (subtask == 3) {
            for (int u = 2; u <= n; ++u)
                ensuref(parent[u] == 1, "Subtask 3 requires a star rooted at 1");
        } else if (subtask != 4) {
            quitf(_fail, "unknown subtask id: %d", subtask);
        }
    }
    return 0;
}
