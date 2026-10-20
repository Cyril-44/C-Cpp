#include <bits/stdc++.h>
using namespace std;

// Usage: snapshot_oracle input answer [uniform_checks=20]
// Validate answers at the first 8 operations, the final operation, and uniformly
// spaced operations. Rebuild from the ACTIVE INTERVAL SET at every snapshot:
// contain[v] = number of intervals containing the leaf interval of v;
// canonical_count[v] = contain[v] - contain[parent[v]].
// A left-endpoint sweep with a Fenwick tree computes all contain[] values.
// This tool shares neither dynamic matrices nor dynamic canonical counters with
// the standard solution. Time per snapshot O((n + active_intervals) log(n+1)),
// memory O(n+q). All traversals are iterative.
static constexpr int MOD = 998244353;
struct State { long long cost; int ways; };
struct Node {
    int left = 0, right = 0, parent = 0, first = 0, last = 0;
    long long weight = 0;
    State required{0, 1}, full{0, 1};
};

bool read_integer(istream& input, long long& result) {
    string text;
    if (!(input >> text) || text.empty()) return false;
    size_t position = 0;
    bool negative = false;
    if (text[position] == '+' || text[position] == '-') {
        negative = text[position] == '-';
        if (++position == text.size()) return false;
    }
    const unsigned long long limit = static_cast<unsigned long long>(LLONG_MAX) + (negative ? 1ULL : 0ULL);
    unsigned long long value = 0;
    for (; position < text.size(); ++position) {
        if (text[position] < '0' || text[position] > '9') return false;
        const unsigned digit = unsigned(text[position] - '0');
        if (value > (limit - digit) / 10ULL) return false;
        value = value * 10ULL + digit;
    }
    if (negative && value == static_cast<unsigned long long>(LLONG_MAX) + 1ULL) result = LLONG_MIN;
    else result = negative ? -static_cast<long long>(value) : static_cast<long long>(value);
    return true;
}

State product(State a, State b) {
    return {a.cost + b.cost, int(1LL * a.ways * b.ways % MOD)};
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto fail = [](const string& message) { cerr << "FAIL: " << message << '\n'; return 1; };
    if (argc != 3 && argc != 4) return fail("usage: snapshot_oracle input answer [uniform_checks=20]");
    long long uniform_checks = 20;
    if (argc == 4) {
        istringstream argument(argv[3]);
        string extra;
        if (!read_integer(argument, uniform_checks) || uniform_checks < 0 || uniform_checks > 100000 || (argument >> extra))
            return fail("uniform_checks must be an integer in [0,100000]");
    }
    ifstream input(argv[1]), answer(argv[2]);
    if (!input || !answer) return fail("cannot open input or answer file");
    long long n_value, q_value;
    if (!read_integer(input, n_value) || !read_integer(input, q_value) ||
        n_value < 1 || n_value > 100000 || q_value < 1 || q_value > 100000)
        return fail("invalid input header");
    const int n = int(n_value), q = int(q_value), vertices = 2 * n - 1;
    vector<Node> tree(vertices + 1);
    for (int v = 1; v <= vertices; ++v) {
        long long left, right, weight;
        if (!read_integer(input, left) || !read_integer(input, right) || !read_integer(input, weight) ||
            left < 0 || left > vertices || right < 0 || right > vertices || weight < 1 || weight > 1000000000LL ||
            ((left == 0) != (right == 0)) || (left != 0 && left == right))
            return fail("invalid data for node " + to_string(v));
        tree[v].left = int(left);
        tree[v].right = int(right);
        tree[v].weight = weight;
        if (left) {
            if (tree[int(left)].parent || tree[int(right)].parent)
                return fail("multiple parents in input tree");
            tree[int(left)].parent = tree[int(right)].parent = v;
        }
    }
    if (tree[1].parent) return fail("root has a parent");
    vector<int> order, stack{1};
    vector<unsigned char> seen(vertices + 1, 0);
    order.reserve(vertices);
    int leaf_number = 0;
    while (!stack.empty()) {
        const int v = stack.back();
        stack.pop_back();
        if (seen[v]) return fail("input tree is cyclic");
        seen[v] = 1;
        order.push_back(v);
        if (tree[v].left == 0) tree[v].first = tree[v].last = ++leaf_number;
        else {
            stack.push_back(tree[v].right);
            stack.push_back(tree[v].left);
        }
    }
    if (int(order.size()) != vertices || leaf_number != n) return fail("input tree is disconnected or has a wrong leaf count");
    vector<int> query_head(n + 1, -1), query_next(vertices + 1, -1);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        const int v = *it;
        if (tree[v].left) {
            tree[v].first = tree[tree[v].left].first;
            tree[v].last = tree[tree[v].right].last;
        }
        query_next[v] = query_head[tree[v].first];
        query_head[tree[v].first] = v;
    }

    unordered_set<uint64_t> active;
    active.reserve(size_t(q) * 2 + 1);
    auto rebuild = [&]() -> State {
        vector<int> interval_head(n + 1, -1), interval_right, interval_next;
        interval_right.reserve(active.size());
        interval_next.reserve(active.size());
        for (uint64_t key : active) {
            const int l = int(key >> 32), r = int(key & 0xffffffffULL);
            interval_next.push_back(interval_head[l]);
            interval_right.push_back(r);
            interval_head[l] = int(interval_right.size()) - 1;
        }
        vector<int> fenwick(n + 1, 0), contain(vertices + 1, 0);
        int inserted = 0;
        for (int l = 1; l <= n; ++l) {
            for (int interval = interval_head[l]; interval != -1; interval = interval_next[interval]) {
                ++inserted;
                for (int x = interval_right[interval]; x <= n; x += x & -x) ++fenwick[x];
            }
            for (int v = query_head[l]; v != -1; v = query_next[v]) {
                int before = 0;
                for (int x = tree[v].last - 1; x > 0; x -= x & -x) before += fenwick[x];
                contain[v] = inserted - before;
            }
        }
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            Node& node = tree[*it];
            const int canonical_count = contain[*it] - contain[node.parent];
            assert(canonical_count >= 0);
            if (node.left == 0) {
                node.full = {node.weight, 1};
                node.required = canonical_count ? node.full : State{0, 1};
                continue;
            }
            const State descendants = product(tree[node.left].required, tree[node.right].required);
            const State chosen{descendants.cost + node.weight, descendants.ways};
            const State omitted = product(tree[node.left].full, tree[node.right].full);
            if (chosen.cost < omitted.cost) node.full = chosen;
            else if (omitted.cost < chosen.cost) node.full = omitted;
            else {
                int ways = chosen.ways + omitted.ways;
                if (ways >= MOD) ways -= MOD;
                node.full = {chosen.cost, ways};
            }
            node.required = canonical_count ? node.full : descendants;
        }
        return tree[1].required;
    };

    vector<bool> checkpoint(q + 1, false);
    for (int step = 1; step <= min(q, 8); ++step) checkpoint[step] = true;
    checkpoint[q] = true;
    for (long long j = 1; j <= uniform_checks; ++j)
        checkpoint[int((j * q + uniform_checks - 1) / uniform_checks)] = true;
    int comparisons = 0;
    size_t maximum_active = 0;
    for (int step = 1; step <= q; ++step) {
        string operation;
        long long l, r;
        if (!(input >> operation) || (operation != "+" && operation != "-") ||
            !read_integer(input, l) || !read_integer(input, r) || l < 1 || l > r || r > n)
            return fail("invalid operation " + to_string(step));
        const uint64_t key = (uint64_t(l) << 32) | uint64_t(r);
        if (operation == "+") {
            if (!active.insert(key).second) return fail("duplicate insertion at operation " + to_string(step));
        } else if (active.erase(key) == 0) return fail("invalid deletion at operation " + to_string(step));
        maximum_active = max(maximum_active, active.size());
        long long cost, ways;
        if (!read_integer(answer, cost) || !read_integer(answer, ways) ||
            cost < 0 || cost > 1000000000LL * n || ways < 0 || ways >= MOD)
            return fail("missing or invalid answer at operation " + to_string(step));
        if (checkpoint[step]) {
            const State expected = rebuild();
            ++comparisons;
            if (cost != expected.cost || ways != expected.ways)
                return fail("operation " + to_string(step) + ": expected " +
                    to_string(expected.cost) + " " + to_string(expected.ways) + ", found " +
                    to_string(cost) + " " + to_string(ways));
        }
    }
    string extra;
    if (input >> extra) return fail("extra token after operations");
    if (answer >> extra) return fail("extra token after answers");
    cerr << "PASS: " << comparisons << " snapshots, maximum active intervals=" << maximum_active << '\n';
    return 0;
}
