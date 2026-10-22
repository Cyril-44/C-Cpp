#pragma once

// 仅供 review 使用：检查共享图、引用次数、严格 3 倍平衡和两个矩。
void audit(const WBLT& tree, int root) {
    vector<int> expected(tree.tr.size());
    vector<bool> seen(tree.tr.size()), freeSlot(tree.tr.size());
    if (root) expected[root] = 1;

    auto visit = [&](auto&& self, int u) -> Info {
        if (seen[u]) return tree.info(u);
        seen[u] = true;
        auto node = tree.tr[u];
        assert(node.refcnt > 0 && node.w > 0);
        assert(0 <= node.sum && node.sum < P);
        assert(0 <= node.moment && node.moment < P);
        assert(0 <= node.tag && node.tag < P);
        if (node.w == 1) {
            assert(!node.ls && !node.rs && !node.moment);
            return tree.info(u);
        }
        assert(node.ls && node.rs);
        ++expected[node.ls];
        ++expected[node.rs];
        Info left = self(self, node.ls), right = self(self, node.rs);
        assert(tree.balanced(left.len, right.len));
        assert(tree.balanced(right.len, left.len));
        Info whole = left + right;
        assert(whole.len == node.w);
        assert(mul(whole.sum, node.tag) == node.sum);
        assert(mul(whole.moment, node.tag) == node.moment);
        return tree.info(u);
    };
    if (root) visit(visit, root);

    int live = 0;
    for (int u : tree.bin) {
        assert(u > 0 && u < int(tree.tr.size()));
        assert(!seen[u] && !freeSlot[u] && !tree.tr[u].refcnt);
        freeSlot[u] = true;
    }
    for (int u = 1; u < int(tree.tr.size()); ++u) {
        if (seen[u]) {
            assert(tree.tr[u].refcnt == expected[u]);
            ++live;
        } else {
            assert(freeSlot[u]);
        }
    }
    assert(live + int(tree.bin.size()) + 1 == int(tree.tr.size()));
    assert(!root || live <= 2 * tree.size(root) - 1);
}
