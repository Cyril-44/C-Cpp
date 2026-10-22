// g++ -std=c++17 -O2 review/wblt_stress.cpp -o wblt_stress
#define main reference_solution_main
#include "../sol/std_wblt.cpp"
#undef main
#include "wblt_audit.hpp"

int main(int argc, char** argv) {
    unsigned seed = argc > 1 ? unsigned(strtoul(argv[1], nullptr, 10)) : 20261023;
    int rounds = argc > 2 ? atoi(argv[2]) : 100;
    int steps = argc > 3 ? atoi(argv[3]) : 1000;
    mt19937 rng(seed);
    long long operations = 0;

    for (int round = 0; round < rounds; ++round) {
        int n = 1 + int(rng() % 257);
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) a[i] = int(rng() % P);
        WBLT tree(n);
        int root = tree.build(a, 1, n);

        for (int step = 0; step < steps; ++step) {
            int l = 1 + int(rng() % n), r = 1 + int(rng() % n);
            if (l > r) swap(l, r);
            if (rng() % 3 == 0) {
                int k = 1 + int(rng() % (r - l + 1));
                int start = 1 + int(rng() % (n - k + 1));
                vector<int> block(a.begin() + start, a.begin() + start + k);

                // 在原序列的共享副本上取种子，允许来源与目标重叠。
                auto [ab, c] = tree.split(tree.clone(root), start + k - 1);
                auto [before, source] = tree.split(ab, start - 1);
                tree.recycle(before);
                tree.recycle(c);
                int replacement = tree.repeat(source, r - l + 1);

                auto [left, rest] = tree.split(root, l - 1);
                auto [drop, right] = tree.split(rest, r - l + 1);
                tree.recycle(drop);
                root = tree.merge(left, tree.merge(replacement, right));
                for (int i = l; i <= r; ++i) a[i] = block[(i - l) % k];
            } else {
                int factor = step % 17 == 0 ? 0 : int(rng() % P);
                auto [rest, right] = tree.split(root, r);
                auto [left, middle] = tree.split(rest, l - 1);
                tree.multiply(middle, factor);
                root = tree.merge(left, tree.merge(middle, right));
                for (int i = l; i <= r; ++i) a[i] = mul(a[i], factor);
                for (int j = 0; j < 4; ++j) {
                    int pos = 1 + int(rng() % n), delta = int(rng() % P);
                    root = tree.addPoint(root, pos, delta);
                    a[pos] = add(a[pos], delta);
                }
            }
            audit(tree, root);

            Info expected;
            for (int i = 1; i <= r; ++i) expected = expected + Info{1, a[i], 0};
            Info got = tree.prefix(root, r);
            assert(got.len == expected.len && got.sum == expected.sum);
            assert(got.moment == expected.moment);
            if (step % 31 == 0) {
                for (int i = 1; i <= n; ++i) {
                    int value = sub(tree.prefix(root, i).sum, tree.prefix(root, i - 1).sum);
                    assert(value == a[i]);
                }
            }
            ++operations;
        }
        tree.recycle(root);
        audit(tree, 0);
    }
    cout << "PASS: " << operations << " operations, seed=" << seed << '\n';
}
