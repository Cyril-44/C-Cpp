// 穷举至 14 个叶子的所有严格 3 倍平衡形状。
// g++ -std=c++17 -O2 review/wblt_exhaustive.cpp -o wblt_exhaustive
#define main reference_solution_main
#include "../sol/std_wblt.cpp"
#undef main
#include "wblt_audit.hpp"

struct Shape { int l, r, n; };
vector<Shape> shapes{{-1, -1, 1}};
vector<int> bySize[15];

int instantiate(WBLT& tree, int id, int& value) {
    Shape s = shapes[id];
    if (s.n == 1) {
        int u = tree.alloc();
        tree.tr[u].w = 1;
        tree.tr[u].sum = ++value;
        return u;
    }
    int left = instantiate(tree, s.l, value), right = instantiate(tree, s.r, value);
    return tree.node(left, right);
}

void verify(WBLT& tree, int root, int len, int period, int factor = 1) {
    audit(tree, root);
    Info expected;
    for (int i = 0; i < len; ++i) {
        expected = expected + Info{1, mul(i % period + 1, factor), 0};
    }
    Info got = tree.info(root);
    assert(got.len == expected.len && got.sum == expected.sum && got.moment == expected.moment);
    tree.recycle(root);
    audit(tree, 0);
}

int main() {
    bySize[1].push_back(0);
    for (int n = 2; n <= 14; ++n) {
        for (int l = 1; l < n; ++l) if (l < 3 * (n - l) && n - l < 3 * l) {
            for (int x : bySize[l]) for (int y : bySize[n - l]) {
                bySize[n].push_back(int(shapes.size()));
                shapes.push_back({x, y, n});
            }
        }
    }
    WBLT tree(32);
    long long splits = 0, joins = 0, repeats = 0;
    for (int n = 1; n <= 14; ++n) for (int id : bySize[n]) {
        for (int cut = 0; cut <= n; ++cut) {
            int value = 0, root = instantiate(tree, id, value);
            auto [left, right] = tree.split(root, cut);
            verify(tree, tree.merge(left, right), n, n);
            ++splits;
        }
        if (n <= 12) for (int len : {n, n + 1, 2 * n - 1, 2 * n, 2 * n + 1, 31}) {
            int value = 0, root = instantiate(tree, id, value);
            int factor = n % 3 == 0 ? 0 : P - 1;
            tree.multiply(root, factor);
            verify(tree, tree.repeat(root, len), len, n, factor);
            ++repeats;
        }
    }
    for (int n = 2; n <= 14; ++n) for (int l = 1; l < n; ++l) {
        for (int x : bySize[l]) for (int y : bySize[n - l]) {
            int value = 0, left = instantiate(tree, x, value), right = instantiate(tree, y, value);
            verify(tree, tree.merge(left, right), n, n);
            ++joins;
        }
    }
    cout << "PASS: " << shapes.size() << " shapes, " << splits << " splits, "
         << joins << " merges, " << repeats << " repeats\n";
}
