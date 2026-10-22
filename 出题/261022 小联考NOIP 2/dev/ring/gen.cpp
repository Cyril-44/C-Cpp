#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    string type = opt<string>("type");
    int n = opt<int>("n");
    long long seed = opt<long long>("seed");
    rnd.setSeed(seed);

    ensuref(4 <= n && n <= 1'000'000, "n must be in [4, 1000000]");
    vector<int> parent(n + 1, 0);

    if (type == "chain") {
        for (int u = 2; u <= n; ++u) parent[u] = u - 1;
    } else if (type == "two") {
        for (int u = 2; u < n; ++u) parent[u] = u - 1;
        parent[n] = 1;
    } else if (type == "star") {
        for (int u = 2; u <= n; ++u) parent[u] = 1;
    } else if (type == "balanced") {
        for (int u = 2; u <= n; ++u) parent[u] = u / 2;
    } else if (type == "random") {
        for (int u = 2; u <= n; ++u) parent[u] = rnd.next(1, u - 1);
    } else if (type == "few" || type == "comb") {
        int leaves = type == "comb" ? (n + 1) / 2 : opt<int>("leaves");
        ensuref(1 <= leaves && leaves <= 8 || type == "comb", "few-leaf generator requires leaves in [1, 8]");
        if (leaves == 1) {
            for (int u = 2; u <= n; ++u) parent[u] = u - 1;
        } else {
            int spine = n - leaves + 1;
            ensuref(spine >= leaves, "need n >= 2*leaves-1 for few-leaf tree");
            for (int u = 2; u <= spine; ++u) parent[u] = u - 1;
            for (int j = 1; j < leaves; ++j) parent[spine + j] = j;
        }
    } else if (type == "broom") {
        int spine = max(2, n / 2);
        for (int u = 2; u <= spine; ++u) parent[u] = u - 1;
        for (int u = spine + 1; u <= n; ++u) parent[u] = spine;
    } else {
        quitf(_fail, "unknown tree type: %s", type.c_str());
    }

    cout << n << '\n';
    for (int u = 2; u <= n; ++u) {
        if (u > 2) cout << ' ';
        cout << parent[u];
    }
    cout << '\n';
    return 0;
}
