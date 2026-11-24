#include "testlib.h"

#include <cstdint>
#include <map>
#include <unordered_set>

using namespace std;

static uint64_t key(int l, int r) {
    return (uint64_t(l) << 32) | uint32_t(r);
}

static bool is_tree_interval(int n, int ql, int qr) {
    int l = 1, r = n;
    while (l != ql || r != qr) {
        int mid = (l + r) / 2;
        if (qr <= mid) r = mid;
        else if (ql > mid) l = mid + 1;
        else return false;
    }
    return true;
}

int main(int argc, char** argv) {
    registerValidation(argc, argv);
    prepareOpts(argc, argv);
    const int subtask = opt<int>("subtask", 10);
    ensuref(1 <= subtask && subtask <= 10, "Invalid subtask");
    const int tid = inf.readInt(1, 10, "Tid");
    (void)tid;
    inf.readSpace();
    const int n = inf.readInt(1, 100000, "n");
    inf.readSpace();
    const int q = inf.readInt(1, 300000, "q");
    inf.readEoln();

    if (subtask == 1) ensuref(n == 1, "Subtask 1 requires n=1");
    if (subtask == 2) ensuref(n <= 8 && q <= 100, "Subtask 2 size limit exceeded");
    if (subtask == 3) ensuref(1LL * n * q <= 2000000, "Subtask 3 product limit exceeded");

    unordered_set<uint64_t> active;
    active.reserve(q);
    map<int, int> disjoint;
    for (int i = 1; i <= q; ++i) {
        const char op = inf.readChar();
        ensuref(op == '+' || op == '-', "Unknown operation at line %d", i + 1);
        inf.readSpace();
        const int l = inf.readInt(1, n, "l");
        inf.readSpace();
        const int r = inf.readInt(l, n, "r");
        inf.readEoln();

        if (subtask == 4) ensuref(l == r, "Subtask 4 requires a singleton at operation %d", i);
        if (subtask == 6) ensuref(l == r || (l == 1 && r == n), "Subtask 6 interval at operation %d", i);
        if (subtask == 7) ensuref(r - l <= 1, "Subtask 7 interval too long at operation %d", i);
        if (subtask == 8) ensuref(op == '+', "Subtask 8 forbids deletion at operation %d", i);
        if (subtask == 9) ensuref(is_tree_interval(n, l, r), "Subtask 9 non-node interval at operation %d", i);

        if (op == '+') {
            ensuref(active.insert(key(l, r)).second, "Duplicate insertion at operation %d", i);
            if (subtask == 5) {
                auto it = disjoint.lower_bound(l);
                ensuref(it == disjoint.end() || r < it->first, "Overlapping intervals at operation %d", i);
                ensuref(it == disjoint.begin() || prev(it)->second < l, "Overlapping intervals at operation %d", i);
                disjoint.emplace_hint(it, l, r);
            }
        } else {
            ensuref(active.erase(key(l, r)) == 1, "Deletion of an absent interval at operation %d", i);
            if (subtask == 5) disjoint.erase(l);
        }
    }
    inf.readEof();
}
