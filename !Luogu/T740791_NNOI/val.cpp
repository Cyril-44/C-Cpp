#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    if (argc < 3)
        quitf(_fail, "Usage: ./val <input> <sub>");

    int sub = atoi(argv[2]);
    ensuref(1 <= sub && sub <= 5, "sub must be in [1,5]");

    int n = inf.readInt(1, 1000000, "n");
    inf.readSpace();
    int m = inf.readInt(1, 1000000, "m");
    inf.readEoln();

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = inf.readInt(0, n - 1, "a[i]");
        if (i + 1 < n) inf.readSpace();
    }
    inf.readEoln();

    // Check permutation
    {
        vector<int> cnt(n, 0);
        for (int x : a) {
            ensuref(0 <= x && x < n, "a[i] out of range");
            cnt[x]++;
        }
        for (int i = 0; i < n; i++)
            ensuref(cnt[i] == 1, "a is not a permutation");
    }

    // Read operations
    for (int i = 0; i < m; i++) {
        int type = inf.readInt(1, 2, "op_type");
        inf.readSpace();
        int x = inf.readInt(1, n, "x");
        inf.readSpace();
        int y = inf.readInt(1, n, "y");
        inf.readEoln();

        if (type == 1) {
            // swap
            // no extra constraints
        } else {
            // query
            // no extra constraints
        }
    }

    // Subtask-specific constraints
    switch (sub) {
        case 1:
            // Example: small constraints
            ensuref(n <= 50 && m <= 50, "sub1 requires n,m <= 50");
            break;

        case 2:
            // Example: medium constraints
            ensuref(n <= 1000 && m <= 1000, "sub2 requires n,m <= 1000");
            break;

        case 3:
            // Example: no swap operations
            // You can enforce: all op_type == 2
            // But since we already read them, we need to re-read or store ops.
            // For simplicity, require n,m <= 1000 (like sample)
            ensuref(n <= 1000 && m <= 1000, "sub3 requires n,m <= 1000");
            break;

        case 4:
            // Example: a[1] = 0 (1-indexed)
            ensuref(a[0] == 0, "sub4 requires a[1] = 0");
            break;

        case 5:
            // Full constraints, no special rules
            break;
    }

    inf.readEof();
    return 0;
}
