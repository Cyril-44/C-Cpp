#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <subtask 1..6> <mode 0..4> [n] [seed]\n";
        return 1;
    }

    int subtask = atoi(argv[1]);
    int mode = atoi(argv[2]);

    // 默认 n
    int n_default;
    switch (subtask) {
        case 1: n_default = 10; break;
        case 4: n_default = 1000; break;
        default: n_default = 200000; break;
    }

    int n = (argc >= 4 ? atoi(argv[3]) : n_default);
    n = max(n, 1);

    if (argc >= 5) {
        rnd.setSeed(atoll(argv[4]));
    }

    vector<pair<int,int>> edges;
    edges.reserve(n - 1);

    auto gen_random_tree = [&]() {
        for (int v = 2; v <= n; v++) {
            int p = rnd.next(1, v - 1);
            edges.push_back({p, v});
        }
    };

    auto gen_chain = [&]() {
        for (int v = 2; v <= n; v++) edges.push_back({v - 1, v});
    };

    auto gen_star = [&]() {
        for (int v = 2; v <= n; v++) edges.push_back({1, v});
    };

    auto gen_broom = [&]() {
        if (n == 1) return;
        int pathLen = max(1, int(n * 0.7));
        pathLen = min(pathLen, n);
        for (int v = 2; v <= pathLen; v++) edges.push_back({v - 1, v});
        for (int v = pathLen + 1; v <= n; v++) {
            int p = rnd.next(1, pathLen);
            edges.push_back({p, v});
        }
    };

    auto gen_skewed = [&]() {
        if (n == 1) return;
        for (int v = 2; v <= n; v++) {
            int L = max(1, v - 5);
            int p = rnd.next(L, v - 1);
            edges.push_back({p, v});
        }
    };

    // 子任务 2：必须链
    if (subtask == 2) {
        gen_chain();
    } else {
        switch (mode) {
            case 0: gen_random_tree(); break;
            case 1: gen_chain(); break;
            case 2: gen_star(); break;
            case 3: gen_broom(); break;
            case 4: gen_skewed(); break;
            default: gen_random_tree(); break;
        }
    }

    // 生成 t_i：1..n 的排列
    vector<int> t(n + 1);

    if (subtask == 3) {
        t[1] = n;
        vector<int> rem;
        for (int x = 1; x <= n - 1; x++) rem.push_back(x);
        shuffle(rem.begin(), rem.end());
        int ptr = 0;
        for (int i = 2; i <= n; i++) t[i] = rem[ptr++];
    } else {
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 1);
        shuffle(perm.begin(), perm.end());
        for (int pos = 0; pos < n; pos++) {
            int node = perm[pos];
            t[node] = pos + 1;
        }
    }

    // 生成 s_i
    vector<int> s(n + 1);
    if (subtask == 5) {
        for (int i = 1; i <= n; i++) s[i] = 1;
    } else {
        for (int i = 1; i <= n; i++) {
            int type = rnd.next(1, 3);
            if (type == 1) s[i] = 1;
            else if (type == 2) s[i] = rnd.next(2, 100);
            else s[i] = rnd.next(1000, 400000);
        }
    }

    // 输出
    println(n);
    for (auto &e : edges) println(e.first, e.second);
    for (int i = 1; i <= n; i++) println(t[i], s[i]);

    return 0;
}
