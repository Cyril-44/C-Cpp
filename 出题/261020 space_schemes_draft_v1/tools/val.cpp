#include <bits/stdc++.h>
using namespace std;

// Usage: val [--input FILE] [--subtask ID] [--max-n N] [--max-q Q]
//            [--max-product NQ] [--exact-n N]
//            [--property general|singleton|disjoint|node|insert-only|
//                        whole-or-singleton|short]
// All supplied restrictions are intersected, including repeated options.
// Read stdin by default. Exit 0 for valid input, 1 for errors.

struct Token {
    string_view text;
    size_t line;
};

class Tokens {
    string data;
    size_t pos = 0;
    size_t line = 1;
public:
    explicit Tokens(istream& input)
        : data(istreambuf_iterator<char>(input), istreambuf_iterator<char>()) {}

    bool next(Token& token) {
        while (pos < data.size() && isspace(static_cast<unsigned char>(data[pos]))) {
            if (data[pos] == '\n') ++line;
            ++pos;
        }
        if (pos == data.size()) return false;
        const size_t begin = pos;
        while (pos < data.size() && !isspace(static_cast<unsigned char>(data[pos]))) ++pos;
        token = {string_view(data).substr(begin, pos - begin), line};
        return true;
    }

    size_t current_line() const { return line; }
};

bool parse_integer(string_view text, long long& result) {
    if (text.empty()) return false;
    size_t i = 0;
    bool negative = false;
    if (text[i] == '+' || text[i] == '-') {
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

bool is_tree_node(int n, int l, int r) {
    int left = 1, right = n;
    while (true) {
        if (l == left && r == right) return true;
        if (left == right) return false;
        const int middle = (left + right) / 2;
        if (r <= middle) right = middle;
        else if (l > middle) left = middle + 1;
        else return false;
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int maximum_n = 100000, maximum_q = 300000, exact_n = 0;
    long long maximum_product = LLONG_MAX;
    string input_path;
    set<string> properties;
    set<int> subtasks;
    auto error = [](const string& message) {
        cerr << "INVALID: " << message << '\n';
        return 1;
    };
    for (int i = 1; i < argc; ++i) {
        const string option = argv[i];
        if (option != "--max-n" && option != "--max-q" &&
            option != "--property" && option != "--input" &&
            option != "--max-product" && option != "--exact-n" && option != "--subtask")
            return error("unknown option " + option);
        if (++i == argc) return error("missing value after " + option);
        const string value = argv[i];
        if (option == "--input") input_path = value;
        else if (option == "--property") properties.insert(value);
        else {
            long long limit;
            if (!parse_integer(value, limit) || limit < 1)
                return error("invalid value for " + option);
            if (option == "--max-product") maximum_product = min(maximum_product, limit);
            else if (option == "--subtask") {
                if (limit > 10) return error("subtask must be between 1 and 10");
                subtasks.insert(int(limit));
            } else if (option == "--max-q") {
                if (limit > 300000) return error("invalid value for " + option);
                maximum_q = min(maximum_q, int(limit));
            } else {
                if (limit > 100000) return error("invalid value for " + option);
                if (option == "--max-n") maximum_n = min(maximum_n, int(limit));
                else {
                    if (exact_n != 0 && exact_n != limit)
                        return error("conflicting --exact-n constraints");
                    exact_n = int(limit);
                }
            }
        }
    }
    for (int subtask : subtasks) {
        switch (subtask) {
            case 1: maximum_n = min(maximum_n, 1); break;
            case 2:
                maximum_n = min(maximum_n, 8);
                maximum_q = min(maximum_q, 100);
                break;
            case 3: maximum_product = min(maximum_product, 2000000LL); break;
            case 4: properties.insert("singleton"); break;
            case 5: properties.insert("disjoint"); break;
            case 6: properties.insert("whole-or-singleton"); break;
            case 7: properties.insert("short"); break;
            case 8: properties.insert("insert-only"); break;
            case 9: properties.insert("node"); break;
            case 10: break;
        }
    }
    for (const string& property : properties) {
        if (property != "general" && property != "singleton" && property != "disjoint" &&
            property != "node" && property != "insert-only" &&
            property != "whole-or-singleton" && property != "short")
            return error("unknown property " + property);
    }

    ifstream input_file;
    istream* input = &cin;
    if (!input_path.empty()) {
        input_file.open(input_path, ios::binary);
        if (!input_file) return error("cannot open input file " + input_path);
        input = &input_file;
    }
    Tokens tokens(*input);
    Token token;
    long long n_value, q_value;
    if (!tokens.next(token) || !parse_integer(token.text, n_value))
        return error("line 1: missing or invalid integer n");
    if (n_value < 1 || n_value > maximum_n)
        return error("line " + to_string(token.line) + ": n is out of range");
    if (exact_n != 0 && n_value != exact_n)
        return error("line " + to_string(token.line) + ": n does not match --exact-n");
    if (!tokens.next(token) || !parse_integer(token.text, q_value))
        return error("header: missing or invalid integer q");
    if (q_value < 1 || q_value > maximum_q)
        return error("line " + to_string(token.line) + ": q is out of range");
    if (n_value * q_value > maximum_product)
        return error("header: n * q exceeds the allowed product");
    const int n = int(n_value), q = int(q_value);

    unordered_set<uint64_t> active;
    active.reserve(size_t(q) * 2 + 1);
    map<int, int> disjoint_intervals;
    for (int step = 1; step <= q; ++step) {
        if (!tokens.next(token))
            return error("operation " + to_string(step) + ": unexpected end of input");
        const size_t operation_line = token.line;
        const string location = "operation " + to_string(step) + ", line " +
            to_string(operation_line) + ": ";
        if (token.text != "+" && token.text != "-")
            return error(location + "operation must be + or -");
        const bool insertion = token.text == "+";
        long long l_value, r_value;
        if (!tokens.next(token) || !parse_integer(token.text, l_value))
            return error(location + "missing or invalid integer l");
        if (!tokens.next(token) || !parse_integer(token.text, r_value))
            return error(location + "missing or invalid integer r");
        if (l_value < 1 || l_value > r_value || r_value > n)
            return error(location + "require 1 <= l <= r <= n");
        const int l = int(l_value), r = int(r_value);
        if (properties.count("singleton") && l != r)
            return error(location + "singleton property requires l = r");
        if (properties.count("whole-or-singleton") && l != r && !(l == 1 && r == n))
            return error(location + "whole-or-singleton requires l = r or [l,r] = [1,n]");
        if (properties.count("short") && r - l + 1 > 2)
            return error(location + "short property requires interval length <= 2");
        if (properties.count("node") && !is_tree_node(n, l, r))
            return error(location + "interval is not a node of the specified segment tree");
        if (properties.count("insert-only") && !insertion)
            return error(location + "insert-only property forbids deletion");

        const uint64_t key = (uint64_t(unsigned(l)) << 32) | unsigned(r);
        if (insertion) {
            if (!active.insert(key).second)
                return error(location + "duplicate insertion of an active interval");
            if (properties.count("disjoint")) {
                auto next = disjoint_intervals.lower_bound(l);
                if (next != disjoint_intervals.end() && next->first <= r)
                    return error(location + "inserted interval overlaps an active interval");
                if (next != disjoint_intervals.begin() && prev(next)->second >= l)
                    return error(location + "inserted interval overlaps an active interval");
                disjoint_intervals.emplace(l, r);
            }
        } else {
            if (active.erase(key) == 0)
                return error(location + "deletion of an interval that is not active");
            if (properties.count("disjoint")) disjoint_intervals.erase(l);
        }
    }
    if (tokens.next(token))
        return error("line " + to_string(token.line) + ": extra token after the last operation");
    cerr << "VALID: n=" << n << ", q=" << q;
    for (int subtask : subtasks) cerr << ", subtask=" << subtask;
    for (const string& property : properties) cerr << ", property=" << property;
    cerr << '\n';
    return 0;
}
