#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Deterministic testlib generator; all parents precede their children.
int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    const string type = opt<string>("type");
    const int n = opt<int>("n");
    rnd.setSeed(opt<long long>("seed"));
    ensuref(4 <= n && n <= 1'000'000, "n must be in [4, 1000000]");
    vector<int> parent(n + 1);

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
        const int leaves = type == "comb" ? (n + 1) / 2 : opt<int>("leaves");
        ensuref(type == "comb" || (1 <= leaves && leaves <= 8),
                "few requires leaves in [1, 8]");
        const int spine = n - leaves + 1;
        ensuref(spine >= leaves, "need n >= 2*leaves-1");
        for (int u = 2; u <= spine; ++u) parent[u] = u - 1;
        for (int j = 1; j < leaves; ++j) parent[spine + j] = j;
    } else if (type == "few_random") {
        const int leaves = opt<int>("leaves");
        ensuref(1 <= leaves && leaves <= 8 && n >= 2 * leaves - 1,
                "few_random requires 1 <= leaves <= 8 and n >= 2*leaves-1");
        vector<int> tips(1, 1);
        int used = 1;
        // Random binary skeleton, followed by randomly interleaved long arms.
        while (static_cast<int>(tips.size()) < leaves) {
            int at = rnd.next(0, static_cast<int>(tips.size()) - 1);
            int p = tips[at];
            parent[++used] = p;
            tips[at] = used;
            parent[++used] = p;
            tips.push_back(used);
        }
        while (used < n) {
            int at = rnd.next(0, leaves - 1);
            parent[++used] = tips[at];
            tips[at] = used;
        }
    } else if (type == "broom") {
        int spine = max(2, n / 2);
        for (int u = 2; u <= spine; ++u) parent[u] = u - 1;
        for (int u = spine + 1; u <= n; ++u) parent[u] = spine;
    } else if (type == "binary_random") {
        vector<int> slots = {1, 1};
        for (int u = 2; u <= n; ++u) {
            int at = rnd.next(0, static_cast<int>(slots.size()) - 1);
            parent[u] = slots[at];
            slots[at] = slots.back();
            slots.pop_back();
            slots.push_back(u);
            slots.push_back(u);
        }
    } else {
        quitf(_fail, "unknown tree type: %s", type.c_str());
    }

    cout << n << '\n';
    for (int u = 2; u <= n; ++u) cout << (u == 2 ? "" : " ") << parent[u];
    cout << '\n';
}
