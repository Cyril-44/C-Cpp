// Enumerate ALL logically weight-balanced binary tree shapes up to 14 leaves.
// Test every split, every ordered join up to 14 total leaves, and shared repeats.
// Compile without -DNDEBUG:
// g++ -std=c++17 -O2 -DAUDIT tools/wblt_exhaustive.cpp -o wblt_exhaustive
#define main reference_solution_main
#include "../solutions/std.cpp"
#undef main

struct Shape { int l, r, n; };
std::vector<Shape> shapes{{-1, -1, 1}};
std::vector<int> by_size[15];

u32 instantiate(Rope& tree, int id, u32& value) {
    Shape s = shapes[id];
    if (s.n == 1) return tree.leaf(++value);
    u32 a = instantiate(tree, s.l, value), b = instantiate(tree, s.r, value);
    u32 root = tree.node(a, b);
    tree.release(a); tree.release(b);
    return root;
}

void verify(Rope& tree, u32 root, int len, int period, u32 factor = 1) {
    tree.audit(root);
    Agg expected;
    for (int i = 0; i < len; ++i) expected = cat(expected, {1, mul(i % period + 1, factor), 0, 0});
    Agg got = tree.agg(root);
    assert(got.n == expected.n && got.s0 == expected.s0 && got.s1 == expected.s1 && got.s2 == expected.s2);
}

int main() {
    by_size[1].push_back(0);
    for (int n = 2; n <= 14; ++n) {
        for (int l = 1; l < n; ++l) if (l <= 3 * (n - l) && n - l <= 3 * l) {
            for (int x : by_size[l]) for (int y : by_size[n - l]) {
                by_size[n].push_back(int(shapes.size()));
                shapes.push_back({x, y, n});
            }
        }
    }
    Rope tree(32);
    u64 splits = 0, joins = 0, repeats = 0;
    for (int n = 1; n <= 14; ++n) for (int id : by_size[n]) {
        for (int cut = 0; cut <= n; ++cut) {
            u32 value = 0, root = instantiate(tree, id, value);
            auto [a, b] = tree.split(root, cut);
            u32 joined = tree.join(a, b);
            tree.release(root); tree.release(a); tree.release(b);
            verify(tree, joined, n, n);
            tree.release(joined); assert(!tree.live); ++splits;
        }
        if (n <= 12) for (int len : {n, n + 1, 2 * n - 1, 2 * n, 2 * n + 1, 31}) {
            u32 value = 0, raw = instantiate(tree, id, value);
            u32 factor = n % 3 == 0 ? 0 : P - 1;
            u32 source = tree.scale(raw, factor), root = tree.repeat(source, len);
            tree.release(raw); tree.release(source);
            verify(tree, root, len, n, factor);
            tree.release(root); assert(!tree.live); ++repeats;
        }
    }
    for (int len = 2; len <= 14; ++len) for (int l = 1; l < len; ++l) {
        for (int x : by_size[l]) for (int y : by_size[len - l]) {
            u32 value = 0, a = instantiate(tree, x, value), b = instantiate(tree, y, value);
            u32 root = tree.join(a, b);
            tree.release(a); tree.release(b);
            verify(tree, root, len, len);
            tree.release(root); assert(!tree.live); ++joins;
        }
    }
    printf("{\"result\":\"PASS\",\"max_leaves\":14,\"balanced_shapes\":%zu,"
           "\"split_rejoin_cases\":%llu,\"ordered_join_cases\":%llu,\"shared_repeat_cases\":%llu}\n",
           shapes.size(), (unsigned long long)splits, (unsigned long long)joins, (unsigned long long)repeats);
}
