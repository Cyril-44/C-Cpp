#include <bits/stdc++.h>
using namespace std;

// Usage: val [--input FILE] [--subtask ID]
// Multiple --subtask options mean intersection, never override earlier limits.
// Exit 0 for valid input, 1 for malformed input or failed subtask restrictions.
struct Token { string_view text; size_t line = 1; };
class Tokens {
    string data;
    size_t position = 0, line = 1;
public:
    explicit Tokens(istream& input)
        : data(istreambuf_iterator<char>(input), istreambuf_iterator<char>()) {}
    bool next(Token& token) {
        while (position < data.size() && isspace(static_cast<unsigned char>(data[position]))) {
            if (data[position] == '\n') ++line;
            ++position;
        }
        if (position == data.size()) return false;
        const size_t first = position;
        while (position < data.size() && !isspace(static_cast<unsigned char>(data[position]))) ++position;
        token = {string_view(data).substr(first, position - first), line};
        return true;
    }
};

bool parse_integer(string_view text, long long& result) {
    if (text.empty()) return false;
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

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto error = [](const string& message) {
        cerr << "INVALID: " << message << '\n';
        return 1;
    };
    set<int> subtasks;
    string input_path;
    for (int i = 1; i < argc; ++i) {
        const string option = argv[i];
        if (option != "--input" && option != "--subtask") return error("unknown option " + option);
        if (++i == argc) return error("missing value after " + option);
        if (option == "--input") input_path = argv[i];
        else {
            long long id;
            if (!parse_integer(argv[i], id) || id < 1 || id > 10)
                return error("subtask must be an integer from 1 to 10");
            subtasks.insert(int(id));
        }
    }
    ifstream input_file;
    istream* input = &cin;
    if (!input_path.empty()) {
        input_file.open(input_path, ios::binary);
        if (!input_file) return error("cannot open input file");
        input = &input_file;
    }
    Tokens tokens(*input);
    Token token;
    auto read_integer = [&](long long& value) {
        return tokens.next(token) && parse_integer(token.text, value);
    };
    long long n_value, q_value;
    if (!read_integer(n_value) || !read_integer(q_value)) return error("header: expected integers n and q");
    if (n_value < 1 || n_value > 100000 || q_value < 1 || q_value > 100000)
        return error("header: require 1 <= n,q <= 100000");
    const int n = int(n_value), q = int(q_value), vertices = 2 * n - 1;
    if (subtasks.count(1) && (n > 7 || q > 100)) return error("subtask 1 requires n <= 7 and q <= 100");
    if (subtasks.count(2) && n_value * q_value > 2000000)
        return error("subtask 2 requires n*q <= 2000000");

    vector<int> left(vertices + 1), right(vertices + 1), indegree(vertices + 1, 0);
    int leaf_count = 0;
    for (int v = 1; v <= vertices; ++v) {
        long long a, b, weight;
        if (!read_integer(a) || !read_integer(b) || !read_integer(weight))
            return error("node " + to_string(v) + ": missing or invalid integer");
        const string location = "node " + to_string(v) + ", line " + to_string(token.line) + ": ";
        if (weight < 1 || weight > 1000000000LL) return error(location + "weight must be in [1,10^9]");
        if (a < 0 || a > vertices || b < 0 || b > vertices)
            return error(location + "child index is out of range");
        if ((a == 0) != (b == 0)) return error(location + "a node must have zero or two children");
        left[v] = int(a);
        right[v] = int(b);
        if (a == 0) ++leaf_count;
        else {
            if (a == b) return error(location + "left and right children must be different");
            if (a == v || b == v) return error(location + "self-loop");
            if (++indegree[int(a)] > 1 || ++indegree[int(b)] > 1)
                return error(location + "a child has multiple parents");
        }
    }
    if (leaf_count != n) return error("tree must have exactly n leaves");
    if (indegree[1] != 0) return error("root 1 must have no parent");
    for (int v = 2; v <= vertices; ++v)
        if (indegree[v] != 1) return error("node " + to_string(v) + " does not have exactly one parent");

    vector<int> order, stack{1}, depth(vertices + 1, 0), first(vertices + 1), last(vertices + 1);
    vector<unsigned char> seen(vertices + 1, 0);
    order.reserve(vertices);
    int leaf_number = 0, maximum_depth = 0;
    while (!stack.empty()) {
        const int v = stack.back();
        stack.pop_back();
        if (seen[v]) return error("tree contains a cycle or repeated reachable vertex");
        seen[v] = 1;
        order.push_back(v);
        if (left[v] == 0) {
            first[v] = last[v] = ++leaf_number;
            maximum_depth = max(maximum_depth, depth[v]);
        } else {
            depth[left[v]] = depth[right[v]] = depth[v] + 1;
            stack.push_back(right[v]);
            stack.push_back(left[v]);
        }
    }
    if (int(order.size()) != vertices) return error("not all vertices are reachable from root 1 (possibly a disconnected cycle)");
    unordered_set<uint64_t> node_intervals;
    if (subtasks.count(5)) node_intervals.reserve(size_t(vertices) * 2 + 1);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        const int v = *it;
        if (left[v]) {
            first[v] = first[left[v]];
            last[v] = last[right[v]];
            const int left_size = last[left[v]] - first[left[v]] + 1;
            const int right_size = last[right[v]] - first[right[v]] + 1;
            if (subtasks.count(3) && left_size != (left_size + right_size + 1) / 2)
                return error("node " + to_string(v) + ": subtask 3 requires the exact floor-midpoint split");
            if (subtasks.count(6) && left[v] && left[left[v]] != 0)
                return error("node " + to_string(v) + ": subtask 6 requires every internal node's left child to be a leaf");
            if (subtasks.count(7) && right_size < left_size)
                return error("node " + to_string(v) + ": subtask 7 requires right subtree size >= left subtree size");
        }
        if (subtasks.count(5))
            node_intervals.insert((uint64_t(unsigned(first[v])) << 32) | unsigned(last[v]));
    }
    if (subtasks.count(4) && maximum_depth > 30)
        return error("subtask 4 requires maximum leaf depth <= 30 (root depth is 0)");

    unordered_set<uint64_t> active;
    active.reserve(size_t(q) * 2 + 1);
    for (int step = 1; step <= q; ++step) {
        if (!tokens.next(token)) return error("operation " + to_string(step) + ": unexpected end of input");
        const string location = "operation " + to_string(step) + ", line " + to_string(token.line) + ": ";
        if (token.text != "+" && token.text != "-") return error(location + "expected + or -");
        const bool insertion = token.text == "+";
        long long l_value, r_value;
        if (!read_integer(l_value) || !read_integer(r_value)) return error(location + "missing or invalid endpoint");
        if (l_value < 1 || l_value > r_value || r_value > n) return error(location + "require 1 <= l <= r <= n");
        if (subtasks.count(9) && r_value - l_value + 1 > 2)
            return error(location + "subtask 9 requires interval length <= 2");
        const uint64_t key = (uint64_t(l_value) << 32) | uint64_t(r_value);
        if (subtasks.count(5) && !node_intervals.count(key))
            return error(location + "subtask 5 requires every requested interval to equal a tree node");
        if (subtasks.count(8) && !insertion) return error(location + "subtask 8 forbids deletions");
        if (insertion) {
            if (!active.insert(key).second) return error(location + "duplicate insertion");
        } else if (active.erase(key) == 0) return error(location + "deletion of an inactive interval");
    }
    if (tokens.next(token)) return error("line " + to_string(token.line) + ": extra token after the final operation");
    cerr << "VALID: n=" << n << ", q=" << q << ", leaf-depth=" << maximum_depth;
    for (int id : subtasks) cerr << ", subtask=" << id;
    cerr << '\n';
    return 0;
}
