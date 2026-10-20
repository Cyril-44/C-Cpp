#include <bits/stdc++.h>
using namespace std;

// Usage: snapshot_oracle input answer [checks=40]
// Maintain only canonical-decomposition counters between snapshots. At each
// snapshot, recompute every DP state from scratch and compare with the answer.
// Snapshots include the first 16 operations, the last operation, and `checks`
// evenly distributed operations (duplicates removed).
// Time O(q log n + n * (checks + 17)), memory O(n + q).
static constexpr int MOD = 998244353;

struct State { int cost, ways; };
struct Node {
    int l, r;
    int left = -1, right = -1;
    int count = 0;
    State required{0, 1}, full{1, 1};
};
vector<Node> tree;

int build(int l, int r) {
    const int v = int(tree.size());
    tree.push_back(Node{l, r});
    if (l != r) {
        const int middle = (l + r) / 2;
        const int left = build(l, middle);
        const int right = build(middle + 1, r);
        tree[v].left = left;
        tree[v].right = right;
    }
    return v;
}

bool update_counter(int v, int ql, int qr, int delta) {
    Node& node = tree[v];
    if (ql <= node.l && node.r <= qr) {
        node.count += delta;
        return node.count >= 0;
    }
    const int middle = (node.l + node.r) / 2;
    if (ql <= middle && !update_counter(node.left, ql, qr, delta)) return false;
    if (qr > middle && !update_counter(node.right, ql, qr, delta)) return false;
    return true;
}

State combine(State a, State b) {
    return {a.cost + b.cost, int(1LL * a.ways * b.ways % MOD)};
}

State recompute() {
    for (int v = int(tree.size()) - 1; v >= 0; --v) {
        Node& node = tree[v];
        if (node.l == node.r) {
            node.full = {1, 1};
            node.required = node.count ? node.full : State{0, 1};
            continue;
        }
        const Node& left = tree[node.left];
        const Node& right = tree[node.right];
        const State children = combine(left.required, right.required);
        const State store{children.cost + 1, children.ways};
        const State omit = combine(left.full, right.full);
        if (store.cost < omit.cost) node.full = store;
        else if (omit.cost < store.cost) node.full = omit;
        else {
            int ways = store.ways + omit.ways;
            if (ways >= MOD) ways -= MOD;
            node.full = {store.cost, ways};
        }
        node.required = node.count ? node.full : children;
    }
    return tree[0].required;
}

bool read_integer(istream& stream, long long& result) {
    string text;
    if (!(stream >> text) || text.empty()) return false;
    size_t i = 0;
    bool negative = false;
    if (text[i] == '-' || text[i] == '+') {
        negative = text[i] == '-';
        if (++i == text.size()) return false;
    }
    const unsigned long long limit = negative
        ? static_cast<unsigned long long>(LLONG_MAX) + 1ULL
        : static_cast<unsigned long long>(LLONG_MAX);
    unsigned long long value = 0;
    for (; i < text.size(); ++i) {
        if (text[i] < '0' || text[i] > '9') return false;
        const unsigned digit = unsigned(text[i] - '0');
        if (value > (limit - digit) / 10ULL) return false;
        value = value * 10ULL + digit;
    }
    if (negative && value == static_cast<unsigned long long>(LLONG_MAX) + 1ULL)
        result = LLONG_MIN;
    else
        result = negative ? -static_cast<long long>(value) : static_cast<long long>(value);
    return true;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto fail = [](const string& message) {
        cerr << "FAIL: " << message << '\n';
        return 1;
    };
    if (argc != 3 && argc != 4)
        return fail("usage: snapshot_oracle input answer [checks=40]");
    long long requested_checks = 40;
    if (argc == 4) {
        istringstream argument(argv[3]);
        string extra;
        if (!read_integer(argument, requested_checks) || requested_checks < 0 || requested_checks > 300000)
            return fail("checks must be an integer between 0 and 300000");
        if (argument >> extra) return fail("extra text after checks");
    }
    ifstream input(argv[1]), answer(argv[2]);
    if (!input || !answer) return fail("cannot open input or answer file");
    long long n_value, q_value;
    if (!read_integer(input, n_value) || !read_integer(input, q_value) ||
        n_value < 1 || n_value > 100000 || q_value < 1 || q_value > 300000)
        return fail("invalid input header");
    const int n = int(n_value), q = int(q_value);
    tree.reserve(size_t(2 * n - 1));
    build(1, n);

    vector<bool> checkpoint(q + 1, false);
    for (int step = 1; step <= min(q, 16); ++step) checkpoint[step] = true;
    checkpoint[q] = true;
    for (long long j = 1; j <= requested_checks; ++j) {
        const int step = int((j * q + requested_checks - 1) / requested_checks);
        checkpoint[step] = true;
    }
    int comparisons = 0;
    for (int step = 1; step <= q; ++step) {
        string operation;
        long long l_value, r_value;
        if (!(input >> operation) || (operation != "+" && operation != "-") ||
            !read_integer(input, l_value) || !read_integer(input, r_value) ||
            l_value < 1 || l_value > r_value || r_value > n)
            return fail("invalid input at operation " + to_string(step));
        if (!update_counter(0, int(l_value), int(r_value), operation == "+" ? 1 : -1))
            return fail("negative canonical counter at operation " + to_string(step));
        long long space, ways;
        if (!read_integer(answer, space) || !read_integer(answer, ways) ||
            space < 0 || space > n || ways < 0 || ways >= MOD)
            return fail("missing or invalid answer at operation " + to_string(step));
        if (checkpoint[step]) {
            const State expected = recompute();
            ++comparisons;
            if (space != expected.cost || ways != expected.ways)
                return fail("operation " + to_string(step) + ": expected " +
                    to_string(expected.cost) + " " + to_string(expected.ways) +
                    ", found " + to_string(space) + " " + to_string(ways));
        }
    }
    string extra;
    if (input >> extra) return fail("extra token after input operations");
    if (answer >> extra) return fail("extra token after answers");
    cerr << "PASS: " << comparisons << " snapshots checked out of " << q << " operations\n";
    return 0;
}
